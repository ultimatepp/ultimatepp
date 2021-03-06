#include "ScatterCtrl.h"

namespace Upp {

void ScatterCtrl::DoShowData()
{
	DataDlg dlg;
	dlg.Init(*this);
	dlg.Run();
}

void DataDlg::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this, t_("Scatter data"));
	Sizeable().Zoomable();
	
	this->pscatter = &scatter;

	tab.Reset();
	series.Clear();
	for(int c = 0; c < scatter.GetCount(); c++)
		if (!IsNull(scatter.GetCount(c))) {
			WithDataSeries <StaticRect> &dataseries = series.Add();
			CtrlLayout(dataseries);
			dataseries.scatterIndex.Hide();
			dataseries.scatterIndex = c;
			tab.Add(dataseries.SizePos(), scatter.GetLegend(c));
		}
	if (tab.GetCount() < 1)
		return;
	OnTab();
	
	bool addedAll = false;
	if (scatter.IsDeletedDataSource(0))
		return;
	DataSource &serie0 = scatter.GetDataSource(0);
	for(int c = 1; c < scatter.GetCount(); c++) {
		if (!IsNull(scatter.GetCount(c) && !scatter.IsDeletedDataSource(c))) {
			DataSource &serie = scatter.GetDataSource(c);
			if (serie0.SameX(serie)) {
				if (!addedAll) {
					addedAll = true;
					WithDataSeries <StaticRect> &dataseries = series.Add();
					CtrlLayout(dataseries);
					dataseries.scatterIndex.Hide();
					dataseries.scatterIndex = -1;
					tab.Add(dataseries.SizePos(), t_("All"));
				}
			}
		}
	}
	
	tab.WhenAction = [=]{OnTab();};
	butOK.WhenAction = [=] {Close();};
}

Value DataDlg::DataSourceX::Format(const Value& q) const 
{
	if (pscatter->IsDeletedDataSource(index))
		return Null;
	if (int(q) >= pscatter->GetDataSource(index).GetCount())
		return Null;
	Value ret = pscatter->GetStringX(index, q);
	if (ret.Is<String>()) {
		String sret = ret;
		sret.Replace("\n", " ");
		return sret;
	}
	return ret;
}

Value DataDlg::DataSourceY::Format(const Value& q) const 
{
	if (pscatter->IsDeletedDataSource(index))
		return Null;
	if (int(q) >= pscatter->GetDataSource(index).GetCount())
		return Null;
	Value ret = pscatter->GetStringY(index, q);
	if (ret.Is<String>()) {
		String sret = ret;
		sret.Replace("\n", " ");
		return sret;
	}
	return ret;
}

void DataDlg::OnTab() 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	ArrayCtrl &data = series[index].data;	
	int scatterIndex = series[index].scatterIndex;
	data.Reset();
	dataSourceYArr.Clear();
	data.MultiSelect().SetLineCy(EditField::GetStdHeight());
	if (scatterIndex >= 0) {
		data.SetVirtualCount(int(scatter.GetCount(scatterIndex)));
		DataSourceY &dataSourceY = dataSourceYArr.Add();
		dataSourceX.pscatter = dataSourceY.pscatter = pscatter;
		dataSourceX.index = dataSourceY.index = scatterIndex;
		String textX = pscatter->GetLabelX() + 
					   (pscatter->GetUnitsX(scatterIndex).IsEmpty() ? "" : " [" + pscatter->GetUnitsX(scatterIndex) + "]"); 
		String textY = pscatter->GetLegend(scatterIndex) + 
					   (pscatter->GetUnitsY(scatterIndex).IsEmpty() ? "" : " [" + pscatter->GetUnitsY(scatterIndex) + "]"); 
		data.AddRowNumColumn(textX).SetConvert(dataSourceX);
		data.AddRowNumColumn(textY).SetConvert(dataSourceY);
	} else {
		data.SetVirtualCount(int(scatter.GetCount(0)));
		dataSourceX.pscatter = pscatter;
		dataSourceX.index = 0;
		String textX = pscatter->GetLabelX() + 
					   (pscatter->GetUnitsX(0).IsEmpty() ? "" : " [" + pscatter->GetUnitsX(0) + "]"); 
		data.AddRowNumColumn(textX).SetConvert(dataSourceX);
		for (int c = 0; c < scatter.GetCount(); ++c) {
			if (!IsNull(scatter.GetCount(c))) {
				DataSourceY &dataY = dataSourceYArr.Add();
				dataY.pscatter = pscatter;
				dataY.index = c;
				String textY = pscatter->GetLegend(c) + 
					   (pscatter->GetUnitsY(c).IsEmpty() ? "" : " [" + pscatter->GetUnitsY(c) + "]"); 		
				data.AddRowNumColumn(textY).SetConvert(dataY);	
			}
		}
	}
	data.WhenBar = [=](Bar &bar) {OnArrayBar(bar);};
}

void DataDlg::ArrayCopy() {
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.SetClipboard(true, true);
}

void DataDlg::ArraySaveToFile(String fileName) {
	int index = tab.Get();
	if (index < 0)
		return;
	
	FileSel fileToSave;
	GuiLock __;
	if (IsNull(fileName)) {
		String name = pscatter->GetTitle() + " " + pscatter->GetLegend(series[index].scatterIndex);
		if (name.IsEmpty())
			name = t_("Scatter plot data");
		fileToSave.PreSelect(ForceExt(name, ".csv"));
		fileToSave.ClearTypes();
		fileToSave.Type(Format(t_("%s file"), t_("Comma separated values (.csv)")), "*.csv");
	    if(!fileToSave.ExecuteSaveAs(t_("Saving plot data"))) {
	        Exclamation(t_("Plot data has not been saved"));
	        return;
	    }
        fileName = fileToSave;
	} 
	ArrayCtrl &data = series[index].data;
	SaveFileBOMUtf8(fileName, data.AsText(AsString, true, pscatter->GetDefaultCSVSeparator(), 
								"\r\n", pscatter->GetDefaultCSVSeparator(), "\r\n"));
}

void DataDlg::ArraySelect() 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.Select(0, data.GetCount(), true);
}

void DataDlg::OnArrayBar(Bar &menu) 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	menu.Add(t_("Select all"), Null, [=] {ArraySelect();}).Key(K_CTRL_A)
								.Help(t_("Select all rows"));
								
	ArrayCtrl &data = series[index].data;							
	int count = data.GetSelectCount();
	if (count == 0)
		menu.Add(t_("No row selected"), Null, Null).Enable(false).Bold(true);
	else {
		menu.Add(Format(t_("Selected %d rows"), count), Null, Null).Enable(false).Bold(true);
		menu.Add(t_("Copy"), ScatterImgP::Copy(), [=] {ArrayCopy();}).Key(K_CTRL_C)
									.Help(t_("Copy selected rows to clipboard"));
		menu.Add(t_("Save to file"), ScatterImgP::Save(), [=] {ArraySaveToFile(Null);}).Key(K_CTRL_S)
									.Help(t_("Save to .csv file"));
	}
}

}