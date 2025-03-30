#include "I18NCtrlLayoutTest.h"

MainDlg::MainDlg()
{
	::CtrlLayout( *this, "I18n Layout Test" );
	
	AddFrame( m_menu );
	m_menu.Set( THISBACK(OnMenu) );
	AddFrame( InsetFrame() );
	
	m_file <<= THISBACK(OnFile);
	m_pallette <<= THISBACK(OnPallette);
	m_printer <<= THISBACK(OnPrinter);
	
	OnChangeLanguage( ::GetCurrentLanguage() );
	
	m_close <<= Rejector( IDCANCEL );
}

void MainDlg::OnFile()
{
	FileSel dlg;
	dlg.ReadOnlyOption();
	dlg.ExecuteOpen( t_("Test") );
	dlg.ExecuteSaveAs( t_("Test") );
	dlg.ExecuteSelectDir( t_("Test") );
}

void MainDlg::OnPallette()
{
	
}

void MainDlg::OnPrinter()
{
	PrinterJob dlg;
	dlg.Execute();
}

void MainDlg::OnMenu(Bar& bar )
{
	bar.Add( t_("Language"), THISBACK(OnLanguage) ).Key( K_F1 );
}

void MainDlg::OnLanguage(Bar& bar )
{
	const int* languages = GetAllLanguages();
	// We have a tremendous amount of redundancy here... we'll use this as a filter.
	// Note that sometimes, the number changes, but it still comes up as 'English'.
	// This is perhaps because we don't have any translations for the other languages.
	VectorMap< String, int > oldLangs;
	for (int i = 0; languages[i] != 0; i++ )
	{
		String langName = ::GetNativeLangName( languages[ i ] );
		if ( oldLangs.Find( langName ) < 0 )
		{
			bar.Add( langName , THISBACK1( OnChangeLanguage, languages[i] ) );
			oldLangs.Add( langName, languages[i] );
		}
	}
}

void MainDlg::OnChangeLanguage(int language )
{
	m_current.SetData( ::GetNativeLangName( language ) );
	::SetLanguage( language );
}

GUI_APP_MAIN
{
	MainDlg dlg;
	dlg.Run();
}

