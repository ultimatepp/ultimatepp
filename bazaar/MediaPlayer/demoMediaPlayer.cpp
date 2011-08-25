#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/MediaPlayer.h>
#include <Controls4U/Controls4U.h>
#include <Media/VolumeCtrl.h>

#define LAYOUTFILE <Media_demo/demoMediaPlayer.lay>
#include <CtrlCore/lay.h>

struct MediaPlayerDemo : public WithMainMediaPlayer<TopWindow> {
typedef MediaPlayerDemo CLASSNAME;	
	void Play() {
		butPlay.Disable();
		butPause.Enable();
		butStop.Enable();
		butFormat.Disable();
		file.Disable();
		seconds.Hide();
		secondsLabel.SetText("0");
		sliderPosition.SetData(0);
		
		if (!player.Play())
			Exclamation(player.GetError());
		
		seconds.SetText("0");
		sliderPosition.SetData(0);
		butPlay.Enable();
		butPause.Disable();
		butStop.Disable();
		butFormat.Enable();
		file.Enable();
		seconds.Show();
	}
	void Stop() {
		player.Stop();
		seconds.SetText("0");
		sliderPosition.SetData(0);
	}
	void Pause() {
		player.Pause();
	}
	void Load() {
		if (!player.Load(~file)) {
			Exclamation(player.GetError());
			return;
		}	
		Layout();
		int duration = int(player.GetDuration());
		if (duration <= 0)
			duration = 1;
		sliderPosition.MinMax(0, duration);
		if (duration < 20) {
			sliderPosition.SetMajorTicks(5);
			sliderPosition.SetMinorTicks(1);
		} else if (duration < 60) {
			sliderPosition.SetMajorTicks(10);
			sliderPosition.SetMinorTicks(5);
		} else if (duration < 60*10) {
			sliderPosition.SetMajorTicks(60);
			sliderPosition.SetMinorTicks(10);
		} else if (duration < 60*60)  {
			sliderPosition.SetMajorTicks(10*60);
			sliderPosition.SetMinorTicks(5*60);
		} else {
			sliderPosition.SetMajorTicks(15*60);
			sliderPosition.SetMinorTicks(5*60);
		}
		sliderPosition.SetData(0);
		sliderPosition.Enable();
		butFormat.Enable();
		butPlay.Enable();
		butStop.Enable();
		butPause.Enable();
	}
	virtual void Close() {
		player.Stop();
		TopWindow::Close();
	}
	void BWEffect(SDLSurface &surface) {
		static int x, y;
		static bool left, up;
		int width = int(surface.GetWidth()/3);
		int height = int(surface.GetHeight()/3);
		int delta = 2;
		
		if (!player.IsPaused()) {
			if (left)
				x -= delta;
			else
				x += delta;
			if (up) 
				y -= delta;
			else
				y += delta;
			if (x < 0) {
				left = false;
				x = 1;
			} else if (x >= surface.GetWidth()-width) {
				left = true;
				x = surface.GetWidth()-width - 1;
			}
			if (y < 0) {
				up = false;
				y = 1;
			} else if (y >= surface.GetHeight()-height) {
				up = true;
				y = surface.GetHeight()-height - 1;
			}	
		}
		for (int r = y; r < y+height; ++r) {
			for (int c = x; c < x+width; ++c) {
				byte bw = Grayscale(surface.GetPixel(c, r));
				surface.DrawPixel(c, r, Color(bw, bw, bw));
			}
		}
		surface.DrawRect(x, y, width, height, 1, Black());
	}
	void MotionDetect(SDLSurface &surface) {
		static Buffer <byte> img0;
		static Buffer <byte> img1;
		static bool which;
		static int prevLen;
		byte *prev, *actual;
		const int delta = 3;
		
		int len = int(surface.GetWidth()*surface.GetHeight()/delta);
		if (len != prevLen) {
			img0.Alloc(len);
			img1.Alloc(len);
		}
		if (which) {
			prev = img0;
			actual = img1;
			which = false;
		} else {
			prev = img1;
			actual = img0;
			which = true;
		}
		
		byte *preva, *actuala;
		actuala = actual;
		for (int r = 0; r < surface.GetHeight(); r+=delta) {
			for (int c = 0; c < surface.GetWidth(); c+=delta) 
				*(actuala++) = Grayscale(surface.GetPixel(c, r));
		}
		if (len != prevLen) {
			prevLen = len;
			return;
		}
		actuala = actual;
		preva = prev;
		for (int r = 0; r < surface.GetHeight(); r+=delta) {
			for (int c = 0; c < surface.GetWidth(); c+=delta) {
				if (abs(*(actuala++) - *(preva++)) > 10)
					surface.FillRect(c, r, delta, delta, Blue());	
			}
		}
	}
	void OnFrame(SDLSurface &surface) {
		surface.Lock();
		if (blackWhite)
			BWEffect(surface); 
		if (showLogo) 
			surface.DrawImage(imLogo, 0, 0, Black());
		if (motionDetect)
			MotionDetect(surface);
		surface.Unlock();
	}
	void OnSecond() {
		double sec = player.GetSecond();
		secondsLabel.SetText(SecondsToString(int(sec), false));
		sliderPosition.SetData(int(sec));
	}
	void OnPause() {
		if (player.IsPaused()) {
			seconds.Show();
			seconds = SecondsToString(player.GetSecond(), false);
		} else
			seconds.Hide();
	}
	void OnSliderPosition() {
		player.SetSecond(int(~sliderPosition));
		seconds = SecondsToString(player.GetSecond(), false);
	}
	void OnSliderVolume() {
		player.SetAudioFactor(int(~sliderVolume)/100.);
	}
	void OnEditSeconds() {
		player.SetSecond(StringToSeconds(seconds));
	}
	void OnRgb() {
		player.SetRGB(rgb);
		motionDetect.Enable(rgb);
		showLogo.Enable(rgb);
		blackWhite.Enable(rgb);
	}
	void CheckOption(int ii, ArrayCtrl *arr) {
		int num = 0;
		for(int i = 0; i < arr->GetCount(); i++) {
			if (arr->Get(i, 0))
				num++;
		}
		if (num > 1) {
			Exclamation("It is possible to play only one stream");
			arr->Set(ii, 0, false);
		}
	}
	/*void OnFullScreen() {
		static Rect last;
		Close();
		if (IsFullScreen()) {
			FullScreen(false);
			SetRect(last);
		} else {
			last = GetRect();
			FullScreen(true);
		}
		OpenMain();
	}*/
	void FormatDialog() {
		WithFormat<TopWindow> format;
		
		CtrlLayoutOK(format, "File format");
		format.duration.SetText(SecondsToString(player.GetDuration(), false));
		format.fileName.SetText(player.GetFileName());
		
		format.arrVideo.SetLineCy(Draw::GetStdFontCy()+4);
		format.arrVideo.AddColumn("Sel");
		format.arrVideo.AddColumn("Codec");
		format.arrVideo.AddColumn("Pixel Format");
		format.arrVideo.AddColumn("Lang");
		format.arrVideo.AddColumn("Width");
		format.arrVideo.AddColumn("Height");
		format.arrVideo.AddColumn("PAR width");
		format.arrVideo.AddColumn("PAR height");
		format.arrVideo.AddColumn("DAR width");
		format.arrVideo.AddColumn("DAR height");
		format.arrVideo.AddColumn("Frame rate fps");
		format.arrVideo.AddColumn("Tbr");
		format.arrVideo.AddColumn("Tbn");
		format.arrVideo.AddColumn("Tbc");				
		format.arrVideo.AddColumn("Bitrate kbps");
		format.arrVideo.AddColumn("Tags");
		format.arrVideo.ColumnWidths("30 100 70 40 45 45 60 60 60 60 80 50 50 50 70 80");
		format.arrVideo.AutoHideSb().HeaderObject().Absolute();
		
		Array<Option> optionVideo;
		for (int i = 0; i < player.videoData.GetCount(); ++i) {
			format.arrVideo.Add(false, player.videoData[i].codec, player.videoData[i].pixFmt,
					player.videoData[i].lang, 
					player.videoData[i].size.cx, player.videoData[i].size.cy,
					player.videoData[i].par.cx,	 player.videoData[i].par.cy, 
					player.videoData[i].dar.cx,  player.videoData[i].dar.cy,
					FormatDouble(player.videoData[i].frameRate.cx/double(player.videoData[i].frameRate.cy), 2),
					FormatDouble(player.videoData[i].tbr.cx/double(player.videoData[i].tbr.cy), 2),
					FormatDouble(player.videoData[i].tbn.cy/double(player.videoData[i].tbn.cx), 2),
					FormatDouble(player.videoData[i].tbc.cy/double(player.videoData[i].tbc.cx), 2),
					int(player.videoData[i].bitrate/1000),
					player.videoData[i].tags);
			Option &option = optionVideo.Add();
			format.arrVideo.SetCtrl(i, 0, option);
			option <<= THISBACK2(CheckOption, i, &format.arrVideo);
		}
		if (player.GetVideo() >= 0)
			format.arrVideo.Set(player.GetVideoId(), 0, true);
		
		format.arrAudio.SetLineCy(Draw::GetStdFontCy()+4);
		format.arrAudio.AddColumn("Sel");
		format.arrAudio.AddColumn("Codec");
		format.arrAudio.AddColumn("Lang");
		format.arrAudio.AddColumn("Channels");
		format.arrAudio.AddColumn("Sample rate Hz");
		format.arrAudio.AddColumn("Bitrate kbps");
		format.arrAudio.AddColumn("Bits");
		format.arrAudio.AddColumn("Tags");
		format.arrAudio.ColumnWidths("30 100 40 60 90 70 40 100");
		format.arrAudio.AutoHideSb().HeaderObject().Absolute();
		
		format.tags.NoEofLine();
		format.tags.SetData(player.GetTags());
		
		Array<Option> optionAudio;
		for (int i = 0; i < player.audioData.GetCount(); ++i) {
			format.arrAudio.Add(false, player.audioData[i].codec, player.audioData[i].lang,
                    player.audioData[i].channels,
                    int64(player.audioData[i].sampleRate), 
                    int(player.audioData[i].bitrate/1000),
                    player.audioData[i].bits,
                    player.audioData[i].tags);
          	Option &option = optionAudio.Add();
			format.arrAudio.SetCtrl(i, 0, option);
			option <<= THISBACK2(CheckOption, i, &format.arrAudio);
		}
		if (player.GetAudio() >= 0)
			format.arrAudio.Set(player.GetAudioId(), 0, true);
		
		format.Sizeable();
		format.Execute();
		
		int idVideo = -1;
		for (int i = 0; i < format.arrVideo.GetCount(); ++i) {
			if (format.arrVideo.Get(i, 0))
				idVideo = i;
		}
		player.SetVideoId(idVideo);
					  
		int idAudio = -1;
		for (int i = 0; i < format.arrAudio.GetCount(); ++i) {
			if (format.arrAudio.Get(i, 0))
				idAudio = i;
		}
		player.SetAudioId(idAudio);	
	}
	void LabelFormatting(String &str, int val) {
		str = SecondsToString(val, false);
	}
	MediaPlayerDemo() {
		CtrlLayout(*this, "MediaPlayer demo");
		Zoomable().Sizeable();
		
		butPlay.Disable();
		butPlay.WhenAction = THISBACK(Play);
		
		butStop.Disable();
		butStop.WhenAction = THISBACK(Stop);
		
		butPause.Disable();
		butPause.WhenAction = THISBACK(Pause);
		
		butFormat.Disable();
		butFormat.WhenAction = THISBACK(FormatDialog);

		file.WhenChange = THISBACK(Load);

		forceAspect = true;
		forceAspect.WhenAction = THISBACK(OnForceAspect);

		fullScreen = false;
		fullScreen.Disable();
		//fullScreen.WhenAction = THISBACK(OnFullScreen);

		player.ForceAspect(forceAspect);
		player.WhenFrame = THISBACK(OnFrame);
		player.WhenSecond = THISBACK(OnSecond);
		player.WhenPause = THISBACK(OnPause);
		player.ShowAudio(true);
		
		sliderPosition.MinMax(0, 100).SetMinorTicksSize(30).Jump(true);
		sliderPosition.SetData(0);
		sliderPosition.WhenAction = THISBACK(OnSliderPosition);
		sliderPosition.LabelFormat = THISBACK(LabelFormatting);
		sliderPosition.Enable(false);
		
		sliderVolume.MinMax(0, 400).Jump(true);
		sliderVolume.SetData(100);
		
		sliderVolume.WhenAction = THISBACK(OnSliderVolume);
		
		seconds.WhenEnter = THISBACK(OnEditSeconds);
		seconds.SetText("0");
		
		secondsLabel.SetText("0");

		rgb = false;
		rgb.WhenAction = THISBACK(OnRgb);

		motionDetect = false;
		motionDetect.Disable();

		showLogo = false;
		showLogo.Disable();
		
		blackWhite = false;
		blackWhite.Disable();
		
		ImageDraw iwLogo(500, 100);
		iwLogo.DrawEllipse(20, 20, 100, 40, Brown());
		iwLogo.DrawText(46, 26, "U++", Arial(25).Bold(), Black()); 
		imLogo = iwLogo;	
	}
	void OnForceAspect() {
		player.ForceAspect(forceAspect);
	}
	Image imLogo;
};

void Run_MediaPlayer() {
	MediaPlayerDemo().Run();
}

