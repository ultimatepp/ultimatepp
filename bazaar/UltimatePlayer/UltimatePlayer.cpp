#include <plugin/Sqlite3/lib/Sqlite3.h>
#include <plugin/Sqlite3/Sqlite3.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <ShellAPI.h>
#include <winnls.h>
#include <time.h>
//#include <wingdi.h>

using namespace Upp;

#include "ProgressButton.h"
#include "StreamCtrl.h"
#include "TaskbarButton.h"
#include "UltimatePlayer.h"
#include "Win32Utils.h"
#include "Build.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

#define IMAGEFILE <UltimatePlayer/UltimatePlayer.iml>
#define IMAGECLASS Images
#include <Draw/iml_source.h>
#define TFILE <UltimatePlayer/UltimatePlayer.t>
#include <Core/t.h>

extern String GetUserLocale(dword type);


const dword UM_FOREGROUND  = RegisterWindowMessage("ULTIMATEPLAYER-MESSAGE-FOREGROUND");
const dword UM_COMMANDLINE = RegisterWindowMessage("ULTIMATEPLAYER-MESSAGE-COMMANDLINE");
AppInstance app("ULTIMATEPLAYER");

String GetTime(int ms)
{
//	return ::Format("%02d:%02d:%02d", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100);
	return ::Format("%02d:%02d", ms / 1000 / 60, ms / 1000 % 60);
}

Value ConvMs::Format(const Value& q) const
{
	int ms = q;
	return GetTime(ms);
}

FileAssoc::FileAssoc()
{
	CtrlLayoutOKCancel(*this, t_("File associations"));
}


Player::Player()
{
	if(sndsys.Open() < 0)
		exit(1);
	
	srand(static_cast<unsigned int>(time(NULL)));
	rebuild_songs_list = true;
	
	mpx_stream.SetSystem(sndsys);
	cd_stream.SetSystem(sndsys);
	stream.SetSystem(sndsys);
	
	Icon(Images::ImgTray());
	LargeIcon(Images::ImgLargeIcon());
	
	//String date = Format("%02d-%02d-%02d %d:%02d", bmYEAR , bmMONTH, bmDAY, bmHOUR, bmMINUTE);

	CtrlLayout(*this, "Ultimate Player v.1.1 - build " + AsString(BUILD));
	
	AddFrame(menu);
	AddFrame(TopSeparatorFrame());
	menu.Set(THISBACK(MenuBar));
	
	play.SetImage(Images::ImgPlayForward());
	stop.SetImage(Images::ImgStop());
	next.SetImage(Images::ImgNext());
	prev.SetImage(Images::ImgPrev());
	
	quit.SetImage(Images::ImgExit());
	totray.SetImage(Images::ImgToTray());

//	list.Theme(3, true, true);
	list.AddIndex();
	list.AddColumn("Playlists").Edit(name);
	list.AddIndex("Played");
	list.Searching(false).Navigating(false).Closing(false);
	list.HorzGrid(0).VertGrid(0);
	list.TabChangesRow(1);
	list.MovingCols(0);
	list.KeepLastRow();
	list.Dragging();
		
	list.WhenMenuBar = THISBACK(PopupListMenuBar);
	list.WhenNewRow = THISBACK(ListNewRow);
	list.WhenInsertRow = THISBACK(ListInsertRow);
	list.WhenUpdateRow = THISBACK(ListUpdateRow);
	list.WhenChangeRow = THISBACK(ListChangeRow);
	list.WhenRemoveRow = THISBACK(ListRemoveRow);
	list.WhenLeftClick = THISBACK(ListChangeRow);

	list.StdAppend = THISBACK(ShowAddPlaylistMenu);

	ReadPlaylists();
	
	left.Vert(system, list);
	spl.Horz(left, pls[0]);
	spl.SetPos(2000);
	left.SetPos(2500);
	
//	system.Theme(3, false, false);
	system.HorzGrid(0).VertGrid(0);
	system.TabChangesRow(0);
	system.AddColumn("System");
	system.MultiSelect(0);
	
	system.WhenChangeRow = THISBACK(SystemChangeRow);
	system.WhenLeftClick = THISBACK(SystemChangeRow);
	system.WhenMenuBar = THISBACK(SystemMenuBar);
	
	system.Set(0, 0, "Explorator");
	AddPlaylistControl(sys);
	FileSystemInfo fsi;
	Array<FileSystemInfo::FileInfo> root = fsi.Find(Null);
	for(int i = 0, j = 1; i < root.GetCount(); i++)
		if(root[i].root_style == FileSystemInfo::ROOT_CDROM)
		{
			system.Set(j++, 0, root[i].filename);
			AddPlaylistControl(sys, -1, true);
		}
	
	cpl = NULL;
	opl = NULL;
	playid = -1;
	prev_playid = -1;
	track = 0;
	
	register_startup = false;
	register_exts.Add("mp3", 0);
	register_exts.Add("wav", 0);
	register_exts.Add("wma", 0);
	register_exts.Add("cda", 0);
	register_exts.Add("ogg", 0);
	register_exts.Add("asf", 0);

	follow_song = false;
	play_last_song = false;
	goto_last_song = true;
	stop_play = false;
	bring_to_front = true;

	//list.InsertFrame(0, tempfrm);
	
	Sizeable().Zoomable();

	tray.Icon(Images::ImgTray()).Tip("Ultimate Player");
	tray <<= THISBACK(Play);
	tray.WhenLeftDouble = THISBACK(ShowPlayer);
	
	info.postext = t_("Position");
	vol.text = t_("Volume");
	
	play <<= THISBACK(Play);
	stop <<= THISBACK(RealStop);
	
	prev <<= THISBACK(OnPlayPrev);
	next <<= THISBACK(OnPlayNext);
	
	info <<= THISBACK(OnSetPos);
	vol <<= THISBACK(OnSetVol);
	
	quit <<= THISBACK(Close);
	   
	vol.SetPos(sndsys.GetVolume());
	
	vol.immediate = true;
		
	totray <<= THISBACK(ToTray);
	
	isplaying = false;

	filesel.Type("Music (*.mp3 *.ogg *.wma *.wav *.asf *.cda)", "*.mp3 *.mp2 *.ogg *.wma *.wav *.asf *.cda");
	filesel.Multi();
	foldersel.Multi();
		
	info.shaded = false;
	info.dst = Color(148,190,239);
	prevpos = 0;
	
	order_mode = ORDER_ASC;
	repeat_mode = REPEAT_NONE;

	//order.SetImage(Images::ImgOrdered());
	//repeat.SetImage(Images::ImgRepeatNone());
	SetOrder();
	SetRepeat();
	
	order <<= THISBACK(OnChangeOrder);
	repeat <<= THISBACK(OnChangeRepeat);

	if(AddCmd(CommandLine()))	
		system.SetCursor(0);
	else
		list.SetCursor(0);

	RegisterExt("mp3");
	curlang = -1;
	SetLang(0);
}

void Player::Initialize()
{
	info.postext = t_("Position");
	vol.text = t_("Volume");
	list.GetColumn(1).Name(t_("Playlists"));
	
	for(int i = 0; i < pls.GetCount(); i++)
	{
		pls[i].GetColumn(2).Name(t_("Title"));
		pls[i].GetColumn(3).Name(t_("Artist"));
		pls[i].GetColumn(4).Name(t_("Album"));
		pls[i].GetColumn(5).Name(t_("Length"));
		pls[i].RebuildToolBar();
	}

	for(int i = 0; i < sys.GetCount(); i++)
	{
		sys[i].GetColumn(2).Name(t_("Title"));
		sys[i].GetColumn(3).Name(t_("Artist"));
		sys[i].GetColumn(4).Name(t_("Album"));
		sys[i].GetColumn(5).Name(t_("Length"));
		sys[i].RebuildToolBar();
	}
	
	list.RebuildToolBar();
	
	next.Tip(t_("Next song"));
	prev.Tip(t_("Previous song"));
	order.Tip(t_("Order mode"));
	repeat.Tip(t_("Repeat mode"));
	quit.Tip(t_("Exit player"));
	totray.Tip(t_("Hide player to tray"));
	play.Tip(t_("Play/Pause"));
	stop.Tip(t_("Stop"));

	menu.Set(THISBACK(MenuBar));
}

Player::~Player()
{
	StorePositions();
}

GridCtrl& Player::GetPlaylist()
{
	return *dynamic_cast<GridCtrl *>(spl.GetLastChild());
}

void Player::SetShellPlaylist()
{
	system.SetCursor(0);
	system.SetFocus();
}

bool Player::IsShellPlaylist(GridCtrl &pl)
{
	return &sys[0] == &pl;
}

bool Player::IsSystemPlaylist(GridCtrl &pl, bool noexplorator)
{
	for(int i = noexplorator ? 1 : 0; i < system.GetCount(); i++)
		if(&sys[i] == &pl)
			return true;
	return false;
}

bool Player::IsCDPlaylist(GridCtrl &pl)
{
	return IsSystemPlaylist(pl, true);
}

int Player::GetCDTrackNum(const String& filename)
{
	/* "...TrackXX.cda" */
	int num = StrInt(filename.Mid(filename.GetLength() - 6, 2));
	return IsNull(num) ? -1 : num - 1;
}

void Player::ReadPlaylists()
{
	WaitCursor wc;
	
	Sql lst(SQL.GetSession());
	if(lst * Select(ID, NAME).From(LISTS).OrderBy(POS))

	while(lst.Fetch())
	{
		list.Add(lst);
		list(2) = 0;
		GridCtrl &grid = AddPlaylistControl(pls);
		Sql pl(SQL.GetSession());
		pl * Select(IDPL, FILENAME, TITLE, ARTIST, ALBUM, LENGTH).From(PLAYLISTS).Where(ID == lst[ID]).OrderBy(POS);
		while(pl.Fetch())
		{
			grid.Add(pl[IDPL],
			         ToSystemCharset(pl[FILENAME]),
			         pl[TITLE],
			         pl[ARTIST],
			         pl[ALBUM],
			         pl[LENGTH]);
			         
			grid(6) = 0;
		}
	}
	if(list.IsEmpty())
	{
		list.Add();
		list.SetLast(1, "Playlist 1");
		Sql sql(SQL);
		sql * Insert(LISTS)(NAME, list.Get(1))(POS, 0);
		AddPlaylistControl(pls);
	}
}

GridCtrl& Player::AddPlaylistControl(Array<GridCtrl> &pls, int n, bool cd)
{
	GridCtrl &pl = n < 0 ? pls.Add() : pls.Insert(n);
	
	pl.AddIndex("Id");
	//pl.AddIndex("Filename");
	pl.AddHiddenColumn("Filename");
	pl.AddColumn("Title", 4);
	pl.AddColumn("Artist", 4);
	pl.AddColumn("Album", 4);
	pl.AddColumn("Length", 2).SetConvert(Single<ConvMs>()).AlignCenter();
	pl.AddIndex("Played");

	pl.SetToolBar();
	pl.Appending()
	  .Inserting()
	  .Removing()
	  .Searching()
	  .Moving()
	  .Navigating();

	pl.Closing(false);
	pl.HorzGrid(false).VertGrid(true);
	pl.TabChangesRow(0);
	pl.MovingRows(true);
	pl.MovingCols(true);
	pl.FullColResizing();
	pl.Dragging();
	pl.Editing(0);
	pl.Indicator();
	pl.ColorRows();

	pl.WhenMenuBar = THISBACK(PopupMenuBar);
	pl.WhenRemoveRow = THISBACK(PLRemoveRow);
	pl.WhenLeftDouble = THISBACK(StopPlay);
	pl.WhenEnter = THISBACK(StopPlay);
	
	pl.StdAppend = THISBACK(ShowAddMusicMenu);
	
	return pl;	
}

bool Player::MakeDefaultPlaylistName(String &name)
{
	name = TrimRight(TrimLeft(name));
	if(name.IsEmpty())
	{
		name = Format("Playlist %d", list.GetCount());
		return true;
	}
	return false;
}

void Player::AddPlaylist(bool append)
{
	if(append)
		list.DoAppend();
	else
		list.DoInsertBefore();	
	
	list.RestoreFocus(); //Splitter::Vert/Horz calls Layout() that grab the focus...
}

void Player::ListInsertRow()
{
	String name = list(1);
	if(MakeDefaultPlaylistName(name))
		list(1) = name;
	SQL * Insert(LISTS)(NAME, name);
	list(0) = (int) SQL.GetInsertedId();
}

void Player::ListNewRow()
{
	int n = list.IsNewRowAppended() ? -1 : list.GetCursorId();
	spl.Horz(left, AddPlaylistControl(pls, n));
	left.Vert(system, list);
	
	list.Set(list.GetNewRowPos(), 2, 0);
}

void Player::ListUpdateRow()
{
	String name = list(1);
	if(MakeDefaultPlaylistName(name))
		list(1) = name;
	
	SQL * ::Update(LISTS)(NAME, name).Where(ID == list(0));
}

void Player::ListRemoveRow()
{
	if(cpl && cpl == &GetPlaylist())
		RealStop();
	SQL.Begin();
	SQL * Delete(PLAYLISTS).Where(ID == list(0));
	SQL * Delete(LISTS).Where(ID == list(0));
	SQL.Commit();
	pls.Remove(list.GetRemovedRowPos());
}

void Player::ListChangeRow()
{
	int c = list.GetCursorId();
	spl.Horz(left, pls[c]);
	left.Vert(system, list);
	list.SetFocus();
}

void Player::SystemChangeRow()
{
	int c = system.GetCursorId();
	bool samepl = (&sys[c] == &GetPlaylist());
	if(!samepl)
	{
		left.Vert(system, list);
		spl.Horz(left, sys[c]);
		system.SetFocus();
	}
}

void Player::PLRemoveRow()
{
	GridCtrl &pl = GetPlaylist();
	if(pl.IsSelectionBegin())
		SQL.Begin();
	SQL * Delete(PLAYLISTS).Where(IDPL == pl(0));
	if((int) pl(0) == playid)
		RealStop();
	if(pl.IsSelectionEnd())
		SQL.Commit();
}

void Player::AddFile(bool append /* = true*/, const String &file)
{
	String filename = ToSystemCharset(file);
	
	bool cd = IsCDFile(filename);
	bool open = false;
	
	if(!cd)
		open = mpx_stream.Open(filename) > 0;
	else
	{
		open = cd_stream.Open(GetCDDrive(filename)) > 0;
		cd_stream.SetTrack(GetCDTrackNum(filename));
	}
	
	if(open)
	{
		GridCtrl &pl = GetPlaylist();
		if(append)
			pl.DoAppend();
		else
			pl.DoInsertBefore();
		
		//pl.SetNewRowOrder();
		
		WString title = cd ? WString(GetFileTitle(file)) : TrimRight(mpx_stream.GetTag("TITLE"));
		if(title.IsEmpty())
			title = WString(GetFileTitle(file));
		
		pl(1) = filename;
		pl(2) = title;
		
		if(cd)
		{
			pl(3) = "";
			pl(4) = "";
			pl(5) = cd_stream.GetLen(cd_stream.GetTrack());
		}
		else
		{
			pl(3) = mpx_stream.GetTag("ARTIST");
			pl(4) = mpx_stream.GetTag("ALBUM");
			pl(5) = mpx_stream.GetMsLen();
		}
		pl(6) = 0;
	
		if(!IsSystemPlaylist(pl))
		{
			SQL * Insert(PLAYLISTS)
				(ID,       list(0))
				(POS,      pl.GetCursor())
				(FILENAME, /*pl(1)*/file)
				(TITLE,    pl(2))
				(ARTIST,   pl(3))
				(ALBUM,    pl(4))
				(LENGTH,   pl(5));
		
			pl(0) = (int) SQL.GetInsertedId();
		}
		else
			pl(0) = pl.GetCount();
			
		pl.RefreshNewRow();
		Sync();
		
		if(!cd)
			mpx_stream.Close();
	}	
}

void Player::AddFiles(bool append /* = true*/)
{
	if(filesel.ExecuteOpen(t_("Select music file")))
	{
		filesel.Close();
		SQL.Begin();
		WaitCursor wc;
		for(int i = 0; i < filesel.GetCount(); i++)
			AddFile(append, filesel.GetFile(i));
		SQL.Commit();
	}
}

void Player::ListAppendFolders(bool append)
{
	if(foldersel.ExecuteSelectDir(t_("Select music folder")))
	{
		foldersel.Close();
		if(append)
			list.DoAppendNoEdit();
		else
			list.DoInsertBeforeNoEdit();
		String plname = foldersel.Get();
		int i = plname.GetLength();
		while(--i >= 0)
		{
			if(plname[i] == '/' || plname[i] == '\\')
				break;
		}		
		list(1) = plname.Mid(i + 1);
		ListInsertRow();
		list.RefreshNewRow();
		SQL.Begin();
		WaitCursor wc;	
		for(int i = 0; i < foldersel.GetCount(); i++)
			AddFiles(true, foldersel.GetFile(i));
		SQL.Commit();
	}	
}

void Player::ListAddFolders()
{
	ListAppendFolders(true);
}

void Player::ListInsertFolders()
{
	ListAppendFolders(false);
}

bool Player::IsMusicFile(String &path)
{
	String ext = ToLower(GetFileExt(path));
	
	if(ext == ".mp3" || ext == ".mp2" ||
	   ext == ".ogg" || ext == ".mid" ||
	   ext == ".wav" || ext == ".wma" ||
	   ext == ".cda" || ext == ".asf")
		return true;
	else
		return false;
}

bool Player::IsCDFile(String &path)
{
	String lpath = ToLower(path);
	String name = lpath.Mid(lpath.GetLength() - 9, 5);
	return lpath.Mid(lpath.GetLength() - 11, 5) == "track" && lpath.Right(3) == "cda";
}

String Player::GetCDDrive(String &path)
{
	return path.Left(2);
}

void Player::AddFiles(bool append, String path)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) 
	{
		String name = ff.GetName();
		
		if(ff.IsFile() && IsMusicFile(name)) 
		{
			AddFile(append, AppendFileName(path, name));
		}
		else if(ff.IsFolder())
		{
			AddFiles(append, AppendFileName(path, name));
	    }
		ff.Next();
	}
}

void Player::AddFolders(bool append /* = true*/)
{
	GridCtrl &pl = GetPlaylist();
	if(foldersel.ExecuteSelectDir(t_("Select music folder")))
	{
		foldersel.Close();
		SQL.Begin();
		WaitCursor wc;	
		for(int i = 0; i < foldersel.GetCount(); i++)
			AddFiles(append, foldersel.GetFile(i));
		SQL.Commit();
	}
}

void Player::ShowAddMusicMenu()
{
	MenuBar::Execute(THISBACK(AddMusicBar));
}

void Player::ShowAddPlaylistMenu()
{
	MenuBar::Execute(THISBACK(AddPlaylistBar));	
}

void Player::StorePositions()
{
	WaitCursor wc;
	
	SQL.Begin();	
	
	Sql updpl("update playlists set pos = ? where idpl = ? and id = ?");
	Sql updlst("update lists set pos = ? where id = ?");
	
	for(int i = 0; i < list.GetCount(); i++)
	{
		updlst.Run(i, list(i, 0));
		GridCtrl &pl = pls[i];
		if(pl.IsOrderChanged())
		{
			for(int j = 0; j < pl.GetCount(); j++)
				updpl.Run(j, pl(j, 0), list(i, 0));
		}
	}
	
	SQL.Commit();	
	
}

void Player::Play()
{
	if(isplaying)
	{
		if(stream.Pause())
		{
			play.SetImage(Images::ImgPause());
			tray.Icon(Images::ImgTrayPlay()).Tip(t_("Playing.."));
		}
		else
		{
			play.SetImage(Images::ImgPlayForward());
			tray.Icon(Images::ImgTrayPause()).Tip(t_("Playing paused"));
		}

		return;		
	}
	
	if(!cpl)
		cpl = &GetPlaylist();
	
	int c = GetPlayRow();

	if(c < 0)
	{
		c = cpl->GetCursor();
		if(c < 0)
		{
			if(cpl->GetCount() > 0)
			{
				cpl->SetCursor(0);
				c = 0;
			}
			else
				return;
		}
	}
	
	String filename = cpl->Get(c, 1);
	
	bool cd = IsCDFile(filename);
	
	if(stream.Play(cd ? GetCDDrive(filename) : filename, cd ? GetCDTrackNum(filename) : -1))
	{
		playid = prev_playid = cpl->Get(c, 0);
		cpl->Set(c, 6, 1);
		isplaying = true;
		play.SetImage(Images::ImgPause());
		tray.Icon(Images::ImgTrayPlay()).Tip(t_("Playing.."));
		tray <<= THISBACK(TrayPlay);
		
		info.SetName((String) cpl->Get(c, 2));
		info.SetBitRate(fceil((stream.GetByteLen() * 8) / stream.GetMsLen()));
		info.KillTime();
		
		KillTimeCallback(0);
		SetTimeCallback(-1000, THISBACK(UpdateProgress), 0);
		KillTimeCallback(1);
		SetTimeCallback(-1000, THISBACK(UpdatePlayButton), 1);
		
		cpl->GetRow(c).Bg(Color(255, 255, 0)).SetFont(Arial(14).Bold());
		cpl->RefreshRow(c);

		if(follow_song && !stop_play)
			GoToSong();
	}
	else
		Exclamation(Format(t_("File %s does not exists"), DeQtf(filename)));
}

void Player::Stop(bool stopstream, bool resetcpl)
{
	KillTimeCallback();
	if(stopstream)
		stream.Stop();
	play.SetImage(Images::ImgPlayForward());
	tray.Icon(Images::ImgTray()).Tip("Ultimate Player");
	tray <<= THISBACK(TrayPlay);
	info.KillName();
	info.SetPos(0);
	isplaying = false;
	prevpos = 0;
	
	if(!cpl)
		return;
	int c = GetPlayRow();
	if(c >= 0)
	{
		opl->GetRow(c).Clear();
		opl->RefreshRow(c);
	}
	if(resetcpl)
	{
		cpl = NULL;
		opl = NULL;
	}
	playid = -1;
}

void Player::TrayPlay()
{
	SetTimeCallback(1000, THISBACK(Play), 2);
}

void Player::TrayStop()
{
	SetTimeCallback(1000, THISBACK(RealStop), 2);
}

void Player::StopPlay()
{
	rebuild_songs_list = true;
	stop_play = true;
	Stop();
	Play();
	stop_play = false;
}

bool Player::PlayDir(bool forward)
{
	if(order_mode == ORDER_SHUFLE && rebuild_songs_list)
	{
		BuildSongsList();
		rebuild_songs_list = false;
	}
	
	if(!cpl) cpl = &GetPlaylist();

	int pr = GetPlayRow();
	
	if(pr < 0 && order_mode != ORDER_SHUFLE)
	{
		Stop();
		Play();
		return true;
	}
	
	int c = -1;
	
	if(order_mode == ORDER_SHUFLE)
	{
		c = GetSongRow();
	}
	else
	{
		if(repeat_mode == REPEAT_ONE)
			c = pr;
		else
			c = forward ? cpl->GetNextRow(pr) 
			            : cpl->GetPrevRow(pr);
	}

	if(c >= 0)
	{
		
		Stop(false, false);
		playid = cpl->Get(c, 0);
		Play();
		return true;
	}
	else
	{
		if(order_mode == ORDER_SHUFLE)
		{
			if(repeat_mode == REPEAT_NONE)
			{
				return stream.IsPlaying();
			}
			else
			{
				rebuild_songs_list = true;
				return PlayDir(forward);
			}
		}
		
		if(repeat_mode == REPEAT_NONE)
		{
			return stream.IsPlaying();
		}
		else if(repeat_mode == REPEAT_PLAYLIST)
		{
			Stop(false, false);
			playid = forward ? cpl->GetFirst(0) : cpl->GetLast(0);
			Play();
			return true;
		}
		else if(repeat_mode == REPEAT_ALL)
		{
			int c = FindPlaylist(*cpl);
			int nc = 0;
			if(c >= 0)
			{
				if(forward)
				{
					nc = c + 1;
					if(nc > list.GetCount() - 1)
						nc = 0;
				}
				else
				{
					nc = c - 1;
					if(nc < 0)
						nc = list.GetCount() - 1;
				}
			}
			
			Stop(true);
			cpl = &pls[nc];
			/* inaczej na nowej liscie bedzie zaczynal od pozycji kursora jesli kursor 
			   jest gdzies ustawiony lub od poczatku listy */
			playid = forward ? cpl->GetFirst(0) : cpl->GetLast(0);
			Play();
			return true;
		}
		return false;
	}
}

int Player::GetPlayRow()
{
	if(playid == -1)
		return -1;
	
	if(!cpl) cpl = &GetPlaylist();
	
	for(int i = 0; i < cpl->GetCount(); i++)
		if((int) cpl->Get(i, 0) == playid)
		{
			opl = cpl;
			return i;
		}

	for(int i = 0; i < pls.GetCount(); i++)
		if(&pls[i] != cpl)
		{
			opl = &pls[i];
			for(int i = 0; i < opl->GetCount(); i++)
				if((int) opl->Get(i, 0) == playid)
					return i;
		}
	return -1;
}

int Player::FindPlaylist(GridCtrl &pl)
{
	for(int i = 0; i < pls.GetCount(); i++)
		if(&(pls[i]) == &pl)
			return i;
	return -1;		
}

int Player::FindPlaylistId(GridCtrl &pl)
{
	for(int i = 0; i < list.GetCount(); i++)
	{
		int cnt = pls.GetCount();
		int cnt1 = list.GetCount();
		int cc = list.GetRowId(i);
		
		if(&(pls[list.GetRowId(i)]) == &pl)
			return i;
	}
	return -1;		
}


void Player::OnSetPos()
{
	stream.SetPos(info.GetPos());
	prevpos = -1;
}

void Player::OnSetVol()
{
	sndsys.SetVolume(vol.GetPos());
}

void Player::UpdateProgress()
{
	if(stream.IsPlaying())
	{
		int curpos = stream.GetBytePos();
		
		if(!info.HasCapture())
			info.SetPos(stream.GetPos());
		
		info.SetTime(stream.GetMsPos(), stream.GetMsLen());
		if(prevpos >= 0)
			info.SetBitRate(((curpos - prevpos)  * 8) /  1000);
		prevpos = curpos;
	}
	else
	{
		info.SetPos(stream.GetPos());
		info.SetTime(stream.GetMsPos(), stream.GetMsLen());
		
		if(!PlayDir(order_mode))
			RealStop();
	}
}

void Player::UpdatePlayButton()
{
	if(!stream.IsPlaying())
		return;
	
	static int n = 0;
	
	if(n == 0)
	{
		if(stream.IsPaused())
			play.SetImage(Images::ImgPlayForwardFlash());
		else
			play.SetImage(Images::ImgPauseFlash());
	}
	else
	{
		if(stream.IsPaused())
			play.SetImage(Images::ImgPlayForward());
		else
			play.SetImage(Images::ImgPause());
	}
	n = 1 - n;
		
}

void Player::ShowPlayer() 
{
	KillTimeCallback(2);
	Show(!IsShown());
	if(IsShown())
		SetForeground();
}


void Player::Serialize(Stream &s)
{
	int version = 1;
	float volume, pos;	
	SerializePlacement(s);
	s / version;
	s % filesel;
	s % foldersel;
	s % spl;
	s % lang;
	s % order_mode;
	s % repeat_mode;
	s % info.time_direction;
	s % register_startup;
	s % register_exts;
	s % follow_song;
	s % bring_to_front;
	s % play_last_song;
	s % goto_last_song;
	
	if(s.IsLoading())
	{
		s % playid;
		s % volume;
		s % pos;		
		
		sndsys.SetVolume(volume);
		vol.SetPos(volume);
		SetLang(lang);
		SetOrder();
		SetRepeat();
		RegisterExts(false);
		
		if(goto_last_song)
		{
			GoTo(true);
			cpl = opl;
		}
		if(play_last_song)
		{
			if(!goto_last_song)
			{
				GoTo(false);
				cpl = opl;
			}
			Play();
			stream.SetPos(pos);
			UpdateProgress();
		}
	}
	else
	{
		GridCtrl &pl = cpl ? *cpl : GetPlaylist();
		int id = (prev_playid < 0 ? (pl.GetCount() > 0 ? pl.Get(0, 0) : -1) : prev_playid);
		s % prev_playid;

		volume = sndsys.GetVolume();
		s % volume;
		pos = stream.GetPos();
		s % pos;
	}
	for(int i = 0; i < pls.GetCount(); i++)
	{
		//s % pls[i];
	}	
}

void Player::MenuBar(Bar &bar)
{
	bar.Add(t_("Files"), THISBACK(FilesBar));
	bar.Add(t_("Options"), THISBACK(OptionsBar));
}

void Player::FilesBar(Bar &bar)
{
	bar.Add(stream.IsPlaying() ? (stream.IsPaused() ? t_("Play") : t_("Pause")) : t_("Play"), THISBACK(Play)).Key(K_SPACE);
	bar.Separator();
	bar.Add(t_("Exit"), THISBACK(Close));
}

void Player::LanguagesBar(Bar &bar)
{
	bar.Add("English", THISBACK1(SetLang, 0)).Check(lang == 0);
	bar.Add("Polski", THISBACK1(SetLang, 1)).Check(lang == 1);
}

void Player::OptionsBar(Bar &bar)
{
	bar.Add(t_("Language"), THISBACK(LanguagesBar));
	bar.Add(t_("File associations"), THISBACK(FileAssociations));
	bar.Add(t_("Go to played file"), THISBACK(GoToSong)).Key(K_F3);
	bar.Separator();
	bar.Add(t_("Follow played file"), THISBACK(FollowSong)).Check(follow_song);
	bar.Add(t_("Play last file on startup"), THISBACK(PlayLastSong)).Check(play_last_song);
	bar.Add(t_("Go to last file on startup"), THISBACK(GotoLastSong)).Check(goto_last_song);
	//bar.Add(t_("Bring player to front when loading from explorer"), THISBACK(BringToFront)).Check(bring_to_front);
}

void Player::AddMusicBar(Bar &bar)
{
	bar.Add(t_("Add files"), THISBACK(PLAddFiles))
	   .Key(K_INSERT);
	bar.Add(t_("Add folder"), THISBACK(PLAddFolders))
	   .Key(K_CTRL_INSERT);
	bar.Separator();
	bar.Add(t_("Insert files"), THISBACK(PLInsertFiles))
	   .Key(K_ALT_INSERT);
	bar.Add(t_("Insert folder"), THISBACK(PLInsertFolders))
	   .Key(K_ALT|K_CTRL_INSERT);
}

void Player::AddPlaylistBar(Bar &bar)
{
	bar.Add(t_("Add playlist"), THISBACK(ListAdd))
	   .Key(K_INSERT);
	bar.Add(t_("Add folder as playlist"), THISBACK(ListAddFolders))
	   .Key(K_CTRL_INSERT);
	bar.Separator();
	bar.Add(t_("Insert playlist"), THISBACK(ListInsert))
	   .Key(K_ALT_INSERT);
	bar.Add(t_("Insert folder as playlist"), THISBACK(ListInsertFolders))
	   .Key(K_ALT|K_CTRL_INSERT);	  
}

void Player::PopupListMenuBar(Bar &bar)
{
	AddPlaylistBar(bar);
	bar.Separator();
	list.RemovingMenu(bar);
	bar.Add(t_("Change playlist name"), THISBACK(ChangePlaylistName)).Key(K_ENTER);
	bar.Separator();
	list.MovingMenu(bar);
	bar.Separator();
	list.SelectMenu(bar);
	//list.ColumnsMenu(bar);
}

void Player::ChangePlaylistName()
{
	list.DoEdit();
}

void Player::PopupMenuBar(Bar &bar)
{
	GridCtrl &pl = GetPlaylist();
	if(!IsCDPlaylist(pl))
	{
		AddMusicBar(bar);
		bar.Separator();	
		pl.RemovingMenu(bar);
		bar.Separator();
	}
	pl.MovingMenu(bar);
	bar.Separator();
	pl.SelectMenu(bar);
	pl.ColumnsMenu(bar);
}

void Player::SystemMenuBar(Bar &bar)
{
	if(system.GetCursor() > 0)
		bar.Add(t_("Load"), THISBACK(LoadCD));
}

void Player::LoadCD()
{
	GridCtrl &pl = GetPlaylist();
	pl.Clear();
	AddFiles(true, system(0));
	cd_stream.Close();
}

void Player::GoTo(bool gothere)
{
	int c = GetPlayRow();
	if(c < 0)
		return;
	
	int id = FindPlaylistId(*opl);
	if(gothere)
	{
		list.GoTo(id);
		opl->GoTo(c);
		opl->SetFocus();
	}
}

void Player::GoToSong()
{
	GoTo(true);
}

void Player::SetLang(int l)
{
	if(curlang == l)
		return;
	
	int cs = CHARSET_UNICODE;
	//int cs = CHARSET_WIN1250 + atoi(GetUserLocale(LOCALE_IDEFAULTANSICODEPAGE)) - 1250;

	switch(l)
	{
		case 0:
		    SetLanguage(LNGC_('E','N','U','S', cs));
			break;
		case 1:
		    SetLanguage(LNGC_('P','L','P','L', cs));
			break;
	}
	
	Initialize();
	Refresh();
	lang = l;
	curlang = l;
}

void Player::Test()
{
	//SendMessage(GetHWND(), WM_FOREGROUND, 0, 0);
	//PromptOK(AsString((int)GetHWND()));
}

void Player::OnChangeOrder()
{
	if(++order_mode > ORDER_SHUFLE)
		order_mode = ORDER_DSC;
			
	SetOrder();	
}

void Player::SetOrder()
{
	switch(order_mode)
	{
		case ORDER_ASC:
			order.SetImage(Images::ImgOrderAsc());
			break;
		case ORDER_DSC:
			order.SetImage(Images::ImgOrderDsc());
			break;
		case ORDER_SHUFLE:
			order.SetImage(Images::ImgOrderShufled());
			rebuild_songs_list = true;
			break;
	}
}

void Player::OnChangeRepeat()
{
	if(++repeat_mode > REPEAT_NONE)
		repeat_mode = REPEAT_ONE;
	
	SetRepeat();	
}

void Player::SetRepeat()
{
	switch(repeat_mode)
	{
		case REPEAT_ONE:
			repeat.SetImage(Images::ImgRepeatOne());
			break;
		case REPEAT_PLAYLIST:
			repeat.SetImage(Images::ImgRepeatPlaylist());
			break;
		case REPEAT_ALL:
			repeat.SetImage(Images::ImgRepeatAll());
			break;
		case REPEAT_NONE:
			repeat.SetImage(Images::ImgRepeatNone());
			break;
	}
	
	if(repeat_mode != REPEAT_ONE)
		rebuild_songs_list = true;
}

void Player::BuildSongsList()
{
	int isplaying = static_cast<int>(stream.IsPlaying());
	int c = isplaying ? GetPlayRow() : -1;		
	
	switch(repeat_mode)
	{
		case REPEAT_NONE:
		case REPEAT_PLAYLIST:
		{
			if(!cpl) cpl = &GetPlaylist();
			for(int i = 0; i < cpl->GetCount(); i++)
			{
				if(opl == cpl && i == c)
					continue;
				cpl->Set(i, 6, 0);
			}
			break;
		}
		case REPEAT_ALL:
		{
			for(int i = 0; i < pls.GetCount(); i++)
			{
				GridCtrl &pl = pls[i];
				int cnt = pls[i].GetCount();
				if(&pl == opl)
				{
					for(int j = 0, n = 0; j < cnt; j++)
						if(i != c)
							pl.Set(j, 6, 0);
				}
				else
					for(int j = 0; j < cnt; j++)
						pl.Set(j, 6, 0);
			}
			break;
		}
	}
}

int Player::GetSongRow0(GridCtrl &pl, int played_col)
{
	songs.Clear();
	
	for(int i = 0; i < pl.GetCount(); i++)
		if((int) pl.Get(i, played_col) == 0)
			songs.Add(i);
	
	int cnt = songs.GetCount();

	if(cnt == 1)
		return songs[0];

	if(cnt == 0)
		return -1;

	return songs[rand() % cnt];
}

int Player::GetSongRow()
{
	switch(repeat_mode)
	{
		case REPEAT_NONE:
		case REPEAT_PLAYLIST:
		{
			return GetSongRow0(*cpl, 6);
		}
		case REPEAT_ALL:
		{
			int pl_row = GetSongRow0(list, 2);
			if(pl_row < 0)
				return -1;
			cpl = &pls[pl_row];
			return GetSongRow0(*cpl, 6);
		}
	}
	return -1;
}


void Player::ToTray()
{
	Hide();
}

void Player::RegisterExt(const char * ext)
{
	String upext = String("up.") + ext;
	String upcmd = GetExeFilePath() + " \"%L\"";
	SetWinRegString(upext, "", String(".") + ext, HKEY_CLASSES_ROOT);
	SetWinRegString("PlayUp", "", upext + "\\shell", HKEY_CLASSES_ROOT);
	SetWinRegString(upcmd, "", upext + "\\shell\\Enqueue\\command", HKEY_CLASSES_ROOT);
	SetWinRegString(upcmd, "", upext + "\\shell\\Open\\command", HKEY_CLASSES_ROOT);
	SetWinRegString(upcmd, "", upext + "\\shell\\Play\\command", HKEY_CLASSES_ROOT);
	SetWinRegString(upcmd, "", upext + "\\shell\\PlayUp\\command", HKEY_CLASSES_ROOT);
}

void Player::RegisterExts(bool force)
{
	if(register_startup || force)
	{
		for(int i = 0; i < register_exts.GetCount(); i++)
			if(register_exts[i] == 1)
				RegisterExt(register_exts.GetKey(i));
	}
}

void Player::FollowSong()
{
	follow_song = !follow_song;
}

void Player::PlayLastSong()
{
	play_last_song = !play_last_song;
}

void Player::GotoLastSong()
{
	goto_last_song = !goto_last_song;
}

void Player::BringToFront()
{
	bring_to_front = !bring_to_front;
}

void Player::FileAssociations()
{
	FileAssoc dlg;
	
	dlg.mp3 = register_exts.Get("mp3");
	dlg.wav = register_exts.Get("wav");
	dlg.wma = register_exts.Get("wma");
	dlg.cda = register_exts.Get("cda");
	dlg.ogg = register_exts.Get("ogg");
	dlg.asf = register_exts.Get("asf");
	dlg.register_startup = (int) register_startup;
	
	if(dlg.Execute() == IDOK)
	{
		register_startup = dlg.register_startup;
		register_exts.Clear();
		register_exts.Add("mp3", dlg.mp3);
		register_exts.Add("wav", dlg.wav);
		register_exts.Add("wma", dlg.wma);
		register_exts.Add("cda", dlg.cda);
		register_exts.Add("ogg", dlg.ogg);
		register_exts.Add("asf", dlg.asf);
		
		if(!register_startup)
			RegisterExts(true);
	}
}

bool Player::AddCmd(const Vector<String> &cmd, bool play)
{
	int cnt = cmd.GetCount();
	if(cnt > 0)
	{
		SetShellPlaylist();			
		for(int i = 0; i < cmd.GetCount(); i++)
			AddFile(true, cmd[i]);
		if(play)
			StopPlay();
		return true;
	}
	return false;
}

LRESULT Player::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == UM_FOREGROUND)
	{
		if(bring_to_front)
		{
			//::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0,      SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
			//SetForeground();
		}
		return true;
	}
	else if(message == UM_COMMANDLINE)
	{
		AddCmd(app.GetCmd());
		return true;
	}
	else if(message == UM_TASKBAR)		
	{
		tray.PassNotification(lParam);
		return true;
    }
	return Ctrl::WindowProc(message, wParam, lParam);	
}

void Player::State(int reason)
{
	if(reason == OPEN)
		tray.Init(GetHWND());
}

GUI_APP_MAIN
{
	if(app.IsAnotherInstance())
	{
		HWND hwnd = GetAppHwnd("ultimateplayer.exe", true);
		if(hwnd)
		{
			::PostMessage(hwnd, UM_FOREGROUND, 0, 0);
			app.PassCmd(hwnd, UM_COMMANDLINE, CommandLine());
		}
		return;
	}
	
	bool nodb = false;
	Sqlite3Session db;
	db.LogErrors(true);
	
	#ifdef flagDEBUG
	db.SetTrace();
	nodb = true;
	#endif	

	FileIn fi("UltimatePlayer.db3");
	if(fi.IsError() || fi.GetSize() <= 0)
	    nodb = true;
	fi.Close();

	if(!db.Open(ConfigFile("UltimatePlayer.db3"))) 
	{
		PromptOK("Can't create or open database file\n");
		return;
	}
		
	SQL = db;

	if(nodb)
	{
		SqlSchema sch(SQLITE3);
		StdStatementExecutor se(db);
		All_Tables(sch);
		if(sch.ScriptChanged(SqlSchema::UPGRADE))
			Sqlite3PerformScript(sch.Upgrade(), se);
		if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) 
		{
			Sqlite3PerformScript(sch.Attributes(), se);
		}
		if(sch.ScriptChanged(SqlSchema::CONFIG)) 
		{
			Sqlite3PerformScript(sch.ConfigDrop(), se);
			Sqlite3PerformScript(sch.Config(), se);
		}
		sch.SaveNormal();		
	}

	Player pl;

	LoadFromFile(pl);
	pl.Run();
	StoreToFile(pl);	
}

