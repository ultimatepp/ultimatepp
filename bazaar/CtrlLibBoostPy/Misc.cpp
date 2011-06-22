#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void export_Prompts()
{
ONCELOCK
{
	def("promptok", &PromptOK, "Prompts OK");
	def("promptokcancel", &PromptOKCancel, "Prompts OK/Cancel");
	def("promptabortretry", &PromptAbortRetry, "Prompts Abort/Retry");
	def("promptabortretryignore", &PromptAbortRetryIgnore, "Prompts Abort/Retry/Ignore");
	def("promptretrycancel", &PromptRetryCancel, "Prompts Retry/Cancel");
	def("promptyesno", &PromptYesNo, "Prompts Yes/No");
	def("promptyesnocancel", &PromptYesNoCancel, "Prompts Yes/No/Cancel");
	def("exclamation", &Exclamation, "Exclamates an Error Dialog");
}
}

END_UPP_NAMESPACE
