#include "ide.h"

static const int MIN_LINE_NUMBER = 1;

class GoToLineDialog : public WithGoToLineLayout<TopWindow> {
public:
	GoToLineDialog(int currentLine, int maxLine)
	{
		CtrlLayoutOKCancel(*this, t_("Go to line"));
		
		lineInformation.SetText(GenerateLineInfo(maxLine));
		lineEdit.Min(MIN_LINE_NUMBER).Max(maxLine);
		lineEdit.SetFocus();
		lineEdit.SetData(currentLine);
		lineEdit.SelectAll();
	}
	
	int GetLine() const
	{
		return static_cast<int>(lineEdit.GetData());
	}
	
private:
	String GenerateLineInfo(int maxLine)
	{
		StringStream ss;
		
		ss << t_("Go to line") << " (" << MIN_LINE_NUMBER << ", " << maxLine << "):";
		
		return ss.GetResult();
	}
};

void Ide::GoToLine()
{
	const int currentLine = editor.GetCurrentLine() + 1;
	
	GoToLineDialog dlg(currentLine, editor.GetLineCount());
	if(dlg.Execute() != IDOK)
		return;
	
	const int newLine = dlg.GetLine();
	if (currentLine == newLine)
		return;
	
	AddHistory();
	editor.SetCursor(editor.GetPos64(newLine - 1));
	editor.SetFocus();
	AddHistory();
}
