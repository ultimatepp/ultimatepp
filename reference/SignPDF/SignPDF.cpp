#include <Core/SSL/SSL.h>
#include <PdfDraw/PdfDraw.h>
#include <RichText/RichText.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	PdfDraw pdf;
	
	PdfSignatureInfo f;

	f.cert = LoadDataFile("certificate.pem");
	f.pkey = LoadDataFile("privatekey.pem");

	f.name = "Mahrai Ziller";
	f.location = "Masaq";
	f.reason = "I'd like to sign this pdf";
	f.contact_info = "Email me!";

	pdf.DrawText(200, 200, "Hello world!", Serif(200), Magenta());
	SaveFile(GetHomeDirFile("invisible_signature.pdf"), pdf.Finish(&f));
	
	SaveFile(GetHomeDirFile("signature_field.pdf"),
	         Pdf(ParseQTF("[9* Hello world!&&&&&&&&[^<<signature>>^ Mahrai Ziller]"),
	         Size(3968, 6074), 200, false, &f));
}
