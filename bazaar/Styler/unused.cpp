#include "Styler.h"

#define BLENDIT 192

void ChMySkin()
{
	ChStdSkin();
	//Override()	
}

void SetStyle(int a, bool full)
{
	switch(a)
	{
		case 1:
		{
			ChMySkin();
			//ChStdSkin();
			//ChClassicSkin();
			SColorPaper_Write(Blend(White(), Black(), BLENDIT-20)); //for lists, edits etc..
			SColorText_Write(LtGray());
			SColorHighlight_Write(Gray());
			SColorHighlightText_Write(White());
			SColorMenu_Write(SColorPaper());
			SColorMenuText_Write(SColorText());
			SColorInfo_Write(SColorPaper());
			SColorInfoText_Write(SColorText());
			SColorMark_Write(SColorText());
			SColorDisabled_Write(Blend(White(), Black(), BLENDIT/2));
			SColorLight_Write(SColorText());
			SColorFace_Write(Blend(White(), Black(), BLENDIT)); //controls face color
			SColorLabel_Write(SColorText());
			SColorShadow_Write(Black());
	
			SColorLtFace_Write(LtGray());
			SColorDkShadow_Write(Gray());
			
			LabelBoxTextColor_Write(SColorText());
			LabelBoxColor_Write(SColorHighlight());
		}
			break;
		default:
		{
			ChStdSkin();
			//ChClassicSkin();
			//ChHostSkin();
		}
			break;
	}
	
	if(!full) return;

	if(1)
	{
		MenuBar::Style& st = MenuBar::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
				
				//area background, seem to be same
				//st.arealook = SColorFace();
				//st.look = SColorFace();
				
				//st.item = SColorPaper();
				//st.look = White();
		
				//popop menu body color
				//st.popupbody = SColorPaper();
				//surrounding menu frame color
				//st.popupframe = Black();
				//popup iconbar back color
				//st.popupiconbar = SColorFace();
				//??
				//ms.topbar = SColorPaper();
				
				//topitem background color
				//st.topitem[0] = Red(); //unseleced
				//st.topitem[1] = Green(); //mouseover
				//st.topitem[2] = Blue(); //selected
				//topitem text color
				//st.topitemtext[0] = Black(); //unselected
				//st.topitemtext[1] = Black(); //mouseover
				//st.topitemtext[2] = Black(); //selected
				//selected item text color
				//st.itemtext = White();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		ToolBar::Style& st = ToolBar::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
				//st.arealook = SColorFace();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		StatusBar::Style& st = StatusBar::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
				//st.look = SColorFace();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		ScrollBar::Style& st = ScrollBar::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
/*
				DfmCtrlsImageLook(st.vupper, DfmCtrlsImg::I_SBVU);
				DfmCtrlsImageLook(st.vthumb, DfmCtrlsImg::I_SBVT, DfmCtrlsImg::SBVI());
				DfmCtrlsImageLook(st.vlower, DfmCtrlsImg::I_SBVL);
				DfmCtrlsImageLook(st.hupper, DfmCtrlsImg::I_SBHU);
				DfmCtrlsImageLook(st.hthumb, DfmCtrlsImg::I_SBHT, DfmCtrlsImg::SBHI());
				DfmCtrlsImageLook(st.hlower, DfmCtrlsImg::I_SBHL);
		
				Sb_night(st.up, CtrlsImg::UA());
				Sb_night(st.up2, CtrlsImg::UA());
				Sb_night(st.down, CtrlsImg::DA());
				Sb_night(st.down2, CtrlsImg::DA());
				Sb_night(st.left, CtrlsImg::LA());
				Sb_night(st.left2, CtrlsImg::LA());
				Sb_night(st.right, CtrlsImg::RA());
				Sb_night(st.right2, CtrlsImg::RA());
*/
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		Splitter::Style& st = Splitter::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
				//st.look = SColorFace();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		//BUTTONS
/*
		Button::Style& bsNormal = Button::StyleNormal().Write();
		Button::Style& bsOk = Button::StyleOk().Write();
		Button::Style& bsScroll = Button::StyleScroll().Write();
		Button::Style& bsEdge = Button::StyleEdge().Write();
*/
		Button::Style& st = Button::StyleNormal().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
/*
				bsNormal = bsNormal.Standard();
				bsOk = bsOk.Standard();
				bsScroll = bsScroll.Standard();
				bsEdge = bsEdge.Standard();
		
				DfmCtrls2ImageLook(bsNormal.look, DfmCtrlsImg2::I_B);
				DfmCtrls2ImageLook(bsOk.look, DfmCtrlsImg2::I_B);
				DfmCtrlsImageLook(bsScroll.look, DfmCtrlsImg::I_SB);
				DfmCtrls3ImageLook(bsEdge.look, DfmCtrlsImg3::I_EB);
			
				for(int i = 0; i < 3; i++)
				{
					bsNormal.textcolor[i] = SColorText();
					bsNormal.monocolor[i] = SColorText();
					bsOk.textcolor[i] = SColorText();
					bsScroll.monocolor[i] = SColorText();
				}
				bsNormal.textcolor[3] = SColorDisabled();
				bsNormal.monocolor[3] = SColorText();
				bsOk.textcolor[3] = SColorDisabled();
				bsScroll.monocolor[3] = SColorText();
*/
				//st.monocolor[0] = st.monocolor[1] = st.monocolor[2] = st.monocolor[3] = SColorFace();
				//Blend(Blend(SColorText, SColorShadow), SColorLabel, 80);
				//st.textcolor[0] = st.textcolor[1] = st.textcolor[2] = SColorLabel();
				//st.textcolor[3] = SColorDisabled();
			}
				break;
			default:
			{
				st = st.Standard();
/*
				bsNormal = bsNormal.Standard();
				bsOk = bsOk.Standard();
				bsScroll = bsScroll.Standard();
				bsEdge = bsEdge.Standard();
*/
			}
				break;
		}
	}

	if(1)
	{
		ButtonOption::Style& st = ButtonOption::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		SpinButtons::Style& st = SpinButtons::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		EditField::Style& st = EditField::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
				//st.focus = Blend(Green(), Black(), BLENDIT/2);
		/*
				st.paper = SColorPaper();
				st.disabled = SColorFace();
				st.invalid = Blend(es.paper, Blue(), BLENDIT/5);
				st.text = SColorText();
				st.textdisabled = SColorDisabled();
				st.selected = SColorHighlight();
				st.selectedtext = SColorHighlightText();
				for(int i = 0; i < 4; i++)
					st.edge[i] = CtrlsImg::EFE();
				st.activeedge = false;
				st.vfm = 2;
		*/
			}
				break;
			default:
			{
				st = st.Standard();
				//st.focus = Blend(Green(), White(), BLENDIT);
			}
				break;
		}
	}

	if(1)
	{
		HeaderCtrl::Style& st = HeaderCtrl::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		DropList::Style& st = DropList::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

	if(1)
	{
		TabCtrl::Style& st = TabCtrl::StyleDefault().Write();
		switch(a)
		{
			case 1:
			{
				st = st.Standard();
			}
				break;
			default:
			{
				st = st.Standard();
			}
				break;
		}
	}

}
