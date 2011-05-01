////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: TestMAPI.cpp
// Description: Simple program for testing CMapiEx
//
// Copyright (C) 2005-2011, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for the community to benefit.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ported to U++ Framework by Koldo. See License.txt file

#include <MAPIEx/MAPIEx.h>
#include <conio.h>

// you may want to change these to valid settings before testing
#define FROM_NAME 				"Support"
#define FROM_EMAIL 				"support@nospam.com"
#define MSG_ATTACHMENT_FOLDER	"c:\\temp"
#define MSG_ATTACHMENT 			"c:\\temp\\pic.jpg"
#define TO_EMAIL 				"noel@nospam.com"
#define TO_EMAIL2 				"noel2@nospam.com"
#define COPY_MSG_FOLDER 		"TestFolder"
#define PERSONAL_FOLDERS 		"Personal Folders"


// Send a message:
void SendTest(MAPIEx& mapi) {
	printf("\n\nSend Message Test");
	MAPIFolder outbox;
	if (!mapi.OpenOutbox(outbox))
		return;
	
	MAPIMessage message;
	if(!message.Create(mapi, outbox, IMPORTANCE_LOW)) 
		return;
	// In an Exchange server senderEmail has to exist and senderName has to be ""
	// If not message.Send() will fail
	message.SetSender(FROM_NAME, FROM_EMAIL);
	message.SetSubject("Subject");

	// user SetBody for ANSI text, SetRTF for HTML and Rich Text
	message.SetBody("Body");

	// here's an HTML body (uncomment to test)
	//message.SetHTML(_T("<html><body><font size=2 color=red face=Arial><span style='font-size:10.0pt;font-family:Arial;color:red'>Body</font></body></html>"));

	// here's an RTF body (uncomment to test)
	//message.SetRTF(_T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033{\\fonttbl{\\f0\\fnil\\fcharset0 Times New Roman;}}{\\colortbl ;\\red255\\green0\\blue0;}{\\*\\generator Msftedit 5.41.15.1507;}\\viewkind4\\uc1\\pard\\cf1\\lang3082\\b\\f0\\fs20 Body\\cf0\\lang1033\\b0\\par}"));

	message.AddRecipient(TO_EMAIL);
	message.AddRecipient(TO_EMAIL2);
	message.AddAttachment(MSG_ATTACHMENT);

	if(message.Send()) 
		printf("\nSent successfully");
}

// To embed images inside of HTML text:
//		-send a message as usual, setting its HTML text
//		-add an <IMG tag with src="cid:<contentID>"
//		-add an attachment using the same CID
void SendCIDTest(MAPIEx& mapi) {
	printf("\n\nSend CID Message Test");
	MAPIFolder outbox;
	if (!mapi.OpenOutbox(outbox))
		return;
	MAPIMessage message;
	if(!message.Create(mapi, outbox, IMPORTANCE_LOW))
		return;
	message.SetSender(FROM_NAME, FROM_EMAIL);
	message.SetSubject("Subject");

	String strCID = "123456789";
	String strImage = "<html><body><IMG alt=\"\" src=\"cid:";
	strImage += strCID;
	strImage += "\" border=0></body></html>";
	message.SetRTF(strImage);

	message.AddRecipient(TO_EMAIL);
	message.AddAttachment(MSG_ATTACHMENT, NULL, strCID);

	if(message.Send()) 
		printf("\nSent successfully");
}

// Read and save messages and their attachments
void ReadMessageTest(MAPIEx& mapi, const String &folderName) {
	printf("\n\nRead and save message test");
	MAPIFolder folder;
	if (folderName.IsEmpty()) {
		if(!mapi.OpenInbox(folder))
			return;
	} else {
		if(!mapi.OpenFolder(folderName, folder)) 		// Instead of Inbox open selected folder
			return;
	}
	
	// use SortContents to sort the messages, default is ascending by PR_MESSAGE_DELIVERY_TIME
	folder.SortContents(TABLE_SORT_DESCEND);
	//mapi.SetUnreadOnly();		
	
	MAPIMessage message;
	while(folder.GetNextMessage(message)) {
		Time receivedTime = message.GetReceivedTime();
		Time submitTime = message.GetSubmitTime();
		Time modifTime = message.GetLastModificationTime();
		String recipients;
		if (message.GetRecipients()) {
			String name, email;
			int type;
			while (message.GetNextRecipient(name, email, type)) {
				if (!recipients.IsEmpty())
					recipients += "; ";
				recipients += Format("'%s' (%s)", name, email);
			}
		}
		printf("\nMessage from '%s' (%s) to %s subject '%s', received time: %s, sent time: %s, modif time: %s", 
				message.GetSenderName().Begin(), message.GetSenderEmail().Begin(), 
				recipients.Begin(), message.GetSubject().Begin(), 
				Format(receivedTime).Begin(), Format(submitTime).Begin(), Format(modifTime).Begin());
		printf("\nBody: %s", message.GetBody(false).Begin());
		//printf("\nHTML: %s", message.GetHTML().Begin());
		if(message.GetAttachmentCount()) {
			printf("\nSaving attachments to %s...", MSG_ATTACHMENT_FOLDER);
			message.SaveAttachment(MSG_ATTACHMENT_FOLDER);
			printf("done");
		}
		static int num;
		String fileName = "c:\\temp\\" + FormatInt(num) + ".msg";
		printf("\nSaving message to %s ...", fileName.Begin());
		message.SaveToFile(fileName);
		num++;
		printf("done");
	}
}

// This function creates a folder (opens if exists) and copies the first unread message if any 
// to this folder
// You can also move and delete the message, but I wanted the sample to be non destructive just 
// in case
void CopyMessageTest(MAPIEx& mapi) {
	printf("\n\nCopy message test");
	MAPIFolder folder;
	if(!mapi.OpenInbox(folder))
		return; 
	//mapi.SetUnreadOnly();

	MAPIMessage message;
	if(folder.GetNextMessage(message)) {
		printf("\nCopying message from '%s' subject '%s'", message.GetSenderName().Begin(), 
															message.GetSubject().Begin());
		printf(", from Inbox to Inbox/%s'", COPY_MSG_FOLDER);
		MAPIFolder subFolder; 
		if(folder.CreateSubFolder(COPY_MSG_FOLDER, subFolder)) 
			folder.CopyMessage(message, subFolder);
	}
}

long CALLBACK OnNewEvent(LPVOID lpvContext, ULONG cNotification, LPNOTIFICATION lpNotifications) {
	String *data = (String *)lpvContext;
	printf("\nReceived new event. Referenced data is %s.", data->Begin());
	printf("\nActivated event was ");
	for (int i = 0; i < int(cNotification); ++i) {
		switch(lpNotifications[i].ulEventType) {
		case fnevNewMail: 		printf("New email");
								break;
		case fnevObjectMoved: 	printf("Object moved");
								break;
		case fnevObjectDeleted: printf("Object deleted");
								break;
		default:				printf("Unknown event!");			
		}
	}
	return 0;
}

void NotificationTest(MAPIEx& mapi) {
	String data = "Data context";
	if(mapi.Notify(OnNewEvent, (void *)&data, fnevNewMail | fnevObjectMoved | fnevObjectDeleted)) { 
		printf(t_("\nWaiting for a new incoming message... (hit a key to cancel)"));
		while(!_kbhit()) 
			Sleep(100);
	}
}

// This function displays your first contact in your Contacts folder
//
// Use GetName to get the name (default DISPLAY NAME, but can be Initials, First Name etc) 
// Use GetEmail to get the email address (named property) 
// Use GetAddress to get the mailing address of CContactAddress::AddressType
// Use GetPhoneNumber supplying a phone number property (ie PR_BUSINESS_TELEPHONE_NUMBER)
// Use GetNotes to get the notes in either plain text (default) or RTF
void ContactsTest(MAPIEx& mapi) {
	printf("\n\nContacts test");
	MAPIFolder contacts;
	if (!mapi.OpenContacts(contacts))
		return;
	
	// sort by name (stored in PR_SUBJECT)
	contacts.SortContents(TABLE_SORT_ASCEND, PR_SUBJECT);

	String strText;
	MAPIContact contact;
	ContactAddress address;
	if(contacts.GetNextContact(contact)) {
		if(contact.GetName(strText)) printf("\nName: '%s'", strText.Begin());
		Time birthday = contact.GetBirthday();
		if(!IsNull(birthday)) printf("\nBirthday: %s", Format(birthday).Begin());
		Time anniversary = contact.GetAnniversary();
		if(!IsNull(anniversary)) printf("\nAnniversary: %s", Format(anniversary).Begin());
		if(contact.GetCategories(strText)) printf("\nCategories: '%s'", strText.Begin());
		if(contact.GetCompany(strText)) printf("\nCompany: '%s'", strText.Begin());
		if(contact.GetEmail(strText)) printf("\nEmail: '%s'", strText.Begin());
		if(contact.GetHomePage(strText)) printf("\nHomePage: '%s'", strText.Begin());
		if(contact.GetIMAddress(strText)) printf("\nIM Address: '%s'", strText.Begin());
		if(contact.GetAddress(address, ContactAddress::BUSINESS)) 
			printf("\n%s\n%s\n%s\n%s\n%s",address.m_strStreet.Begin(),
				address.m_strCity.Begin(), address.m_strStateOrProvince.Begin(),
				address.m_strCountry.Begin(), address.m_strPostalCode.Begin());
		if(contact.GetAddress(address, ContactAddress::HOME)) 
			printf("\n%s\n%s\n%s\n%s\n%s",address.m_strStreet.Begin(), 
				address.m_strCity.Begin(), address.m_strStateOrProvince.Begin(),
				address.m_strCountry.Begin(), address.m_strPostalCode.Begin());
		if(contact.GetPhoneNumber(strText, PR_BUSINESS_TELEPHONE_NUMBER)) 
			printf("\nPhone: %s", strText.Begin());
		strText = contact.GetBody();
		if(!strText.IsEmpty()) 
			printf("\nNotes: %s", strText.Begin());

		if(contact.HasPicture()) {
			#define DEFAULT_BUFFER_SIZE	4096
			// shows how to read the bytes in, could save this or stream it to memory etc
			MAPIAttachment attachment;
			if(contact.GetAttachment(attachment, 0)) {
				if(attachment.OpenStream()) {
					BYTE buf[DEFAULT_BUFFER_SIZE];
					int nRead, nSize = 0;

					do {
						nRead=attachment.Read(buf, DEFAULT_BUFFER_SIZE);
						if(nRead>0) nSize += nRead;
					} while (nRead>0);
					attachment.CloseStream();
					printf("Contact has picture (%d bytes)\n", nSize);
				}
			}
		}
	}
}

// This function creates a message in the drafts folder and opens it up using the default
// IMessage form
// NOTE: on new messages if ShowForm returns IDOK, the message was sent, the message will not 
//			be valid anymore, 
//       if it returns IDCANCEL, the user closed the dialog and you must save the message or 
//			it will be lost
void CreateDraftTest(MAPIEx& mapi) {
	printf("\n\nCreate Draft Test");
	MAPIFolder drafts;
	if(!mapi.OpenDrafts(drafts))
		return; 
	
	MAPIMessage message;
	if(!message.Create(mapi, drafts, IMPORTANCE_LOW)) 
		return;
	message.SetSender(FROM_NAME, FROM_EMAIL);
	message.SetSubject("Subject");
	message.AddRecipient(TO_EMAIL);

	int nResult = message.ShowForm(&mapi, drafts);
	if(nResult == IDOK) 
		printf("\nDraft sent successfully");
	else if(nResult == IDCANCEL) {
		message.Save();
		printf("\nDraft created successfully");
	}
}

void SelectContactsTest(MAPIEx& mapi) {
	printf("\n\nSelect Contacts Test");
	Vector <String> emails;
	if(mapi.SelectContacts(emails, "ShowAddressBook Test")) {
		for(int i = 0; i < emails.GetCount(); i++) 
			printf("\nAddressBook recipient %d: %s", i+1, emails[i].Begin());
	}
}

// This function creates a contact and sets its name and email
void CreateContactTest(MAPIEx& mapi) {
	printf("\n\nCreate Contact Test");
	MAPIFolder contacts;
	if(!mapi.OpenContacts(contacts)) 
		return;
	
	MAPIContact contact;
	if(contact.Create(mapi, contacts)) {
		contact.SetDisplayNamePrefix("Mr.");
		contact.SetName(FROM_NAME, PR_GIVEN_NAME);
		contact.SetEmail(FROM_EMAIL, 1);
		//contact.SetEmailDisplayAs(String(FROM_NAME) + " (" + FROM_EMAIL + ")", 1);
		contact.UpdateDisplayName();
		if(contact.Save()) 
			printf("\nContact created successfully");
	}	
}

// This function opens the first subfolder under contacts and if successful displays the 
// first contact if any
void ContactSubFolderTest(MAPIEx& mapi) {
	printf("\n\nContact SubFolder Test");
	MAPIFolder contacts;
	if(!mapi.OpenContacts(contacts)) 
		return;
	
	String strFolder;
	MAPIFolder folder;
	if(contacts.GetNextSubFolder(folder, strFolder)) {
		printf("\nContact Sub Folder: %s", strFolder.Begin());
		String strText;
		MAPIContact contact;
		ContactAddress address;
		if(folder.GetNextContact(contact)) {
			if(contact.GetName(strText)) 
				printf("\nContact Name '%s'", strText.Begin());
		}
	}
}

void AppointmentTest(MAPIEx& mapi) {
	printf("\n\nAppointment Test");
	MAPIFolder appointments;
	if(!mapi.OpenCalendar(appointments)) 
		return;
		
	String strText;
	MAPIAppointment appointment;
	if (appointments.GetNextAppointment(appointment)) {
		if(appointment.GetSubject(strText))	
			printf("\nSubject: %s", strText.Begin());
		if(appointment.GetLocation(strText)) 
			printf("\nLocation: %s", strText.Begin());
		Time start = appointment.GetStartTime();
		if(!IsNull(start))	
			printf("\nStart: %s", Format(start).Begin());
		Time end = appointment.GetEndTime();
		if(!IsNull(end))	
			printf("\nEnd: %s", Format(end).Begin());
	}
}

void EnumerateSubFolders(MAPIEx& mapi, MAPIFolder& folder, String parentName) {
	String subFolderName;
	MAPIFolder subFolder;
	while(folder.GetNextSubFolder(subFolder, subFolderName)) {
		String folderName = parentName + "/" + subFolderName;
		printf("\nFolder: %s (%d elems)", folderName.Begin(), subFolder.GetCount());
		EnumerateSubFolders(mapi, subFolder, folderName);
	}
} 

void FolderList(MAPIEx& mapi) {
	printf("\n\nFolder list");
	MAPIFolder folder;
	if(mapi.OpenRootFolder(folder)) 
		EnumerateSubFolders(mapi, folder, "");
}

CONSOLE_APP_MAIN
{
	MAPIEx mapi;
	if(!mapi.Login()) {
		printf("\nFailed to initialize MAPI");
		return;
	}
	printf("\nProfile Name: %s", mapi.GetProfileName().Begin());
	printf("\nProfile Email: %s", mapi.GetProfileEmail().Begin());

	printf("\n\nStore list:");
	int msc = mapi.GetMessageStoreCount();
	for (int i = 0; i < msc; ++i) {
		String name;
		bool isdefault;
		if(mapi.GetMessageStoreProperties(i, name, isdefault))
			printf("\n%d: %s %s", i, name.Begin(), isdefault ? "(default)" : "");	
		else
			printf("\n%d: Problem in store", i);	
	}
	
	// Opening default store
	if(!mapi.OpenMessageStore()) {
		printf("\nFailed to open store");
		return;
	}
	FolderList(mapi);
	
	printf("\nPress a key to continue...");	getchar();
	
	// uncomment the functions you're interested in and/or step through these to see how each thing works.
	//ReadMessageTest(mapi);
	//CopyMessageTest(mapi);
	//CreateContactTest(mapi);
		//ContactsTest(mapi);
	//SelectContactsTest(mapi);
	//CreateDraftTest(mapi);
	//SendTest(mapi);
	//SendCIDTest(mapi);
	//NotificationTest(mapi);
	//ContactSubFolderTest(mapi);
	//AppointmentTest(mapi);

	printf("\nPress a key to end...");		getchar();
}
