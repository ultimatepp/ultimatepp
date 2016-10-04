#include <stdapp/stdapp.hpp>

void stdapp::File(Bar& bar)
{
    bar.Add(t_("New"), THISBACK(OnNew)).Key(K_CTRL_N).Help(t_("Create a new object"));
    bar.Add(t_("Open ..."), THISBACK(OnLoad)).Key(K_CTRL_L).Help(t_("Open an existing object"));
    bar.MenuSeparator();
    bar.Add(t_("Save"), THISBACK(OnSave)).Key(K_CTRL_S).Help(t_("Save the current object"));
    bar.Add(t_("Save as ..."), THISBACK(OnSaveAs)).Key(K_CTRL_A).Help(t_("Save the current object under a different name"));
    bar.MenuSeparator();
    bar.Add(t_("Print ..."), THISBACK(OnPrint)).Key(K_CTRL_P).Help(t_("Print the current object"));
    bar.MenuSeparator();
    bar.Add(t_("Exit"), THISBACK(Exit)).Key(K_CTRL_Q).Help(t_("Exit the application"));
}

void stdapp::OnNew()
{
    Exclamation("You still have to write your own \"New\" code!");
}

void stdapp::OnLoad()
{
    static FileSelector fs;

    if(fs.ExecuteOpen())
    {
        String file_name = fs;
        Exclamation("You still have to complete your own \"OnLoad\" code!");
    }
    else
        status.Temporary(t_("Opening aborted"));
}

void stdapp::OnSave()
{
    Exclamation("You still have to write your own \"Save\" code!");
}

void stdapp::OnSaveAs()
{
    static FileSelector fs;

    if(fs.ExecuteSaveAs())
    {
        String file_name = fs;
        Exclamation("You still have to complete your own \"Save-as\" code!");
    }
    else
        status.Temporary(t_("Saving-as aborted"));
}

void stdapp::OnPrint()
{
    Exclamation("You still have to write your own \"Print\" code!");
}

void stdapp::Exit()
{
    Close();
}
