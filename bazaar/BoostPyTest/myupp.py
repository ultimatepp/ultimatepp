import upp;

# some calls directly to embedded python, called when this module is imported
print upp.app.title;

# some function definitions, can be called from embedded python
def chtitle():
	upp.app.title = "The title was changed from outside :)"
