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
	puts("\nSend Message Test");
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
	//message.SetHTML("<html><body><font size=2 color=red face=Arial><span style='font-size:10.0pt;font-family:Arial;color:red'>Body</font></body></html>");

	// here's an RTF body (uncomment to test)
	//message.SetRTF(_T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033{\\fonttbl{\\f0\\fnil\\fcharset0 Times New Roman;}}{\\colortbl ;\\red255\\green0\\blue0;}{\\*\\generator Msftedit 5.41.15.1507;}\\viewkind4\\uc1\\pard\\cf1\\lang3082\\b\\f0\\fs20 Body\\cf0\\lang1033\\b0\\par}"));

	message.AddRecipient(TO_EMAIL);
	message.AddRecipient(TO_EMAIL2);
	message.AddAttachment(MSG_ATTACHMENT);

	if(message.Send()) 
		puts("Sent successfully");
}

// To embed images inside of HTML text:
//		-send a message as usual, setting its HTML text
//		-add an <IMG tag with src="cid:<contentID>"
//		-add an attachment using the same CID
void SendCIDTest(MAPIEx& mapi) {
	puts("\nSend CID Message Test");
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
		puts("Sent successfully");
}

// Read and save messages and their attachments
void ReadMessageTest(MAPIEx& mapi, const String &folderName = "") {
	puts("\nRead and save message test");
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
				String stype; 
				if (type == MAPI_TO)
					stype = "TO";
				else if (type == MAPI_CC)
					stype = "CC";
				else if (type == MAPI_BCC)
					stype = "BCC";
				else
					stype = "Unknown!";
				recipients += Format("'%s' (%s)(%s)", name, email, stype.Begin());
			}
		}
		puts(Format("Message from '%s' (%s) to %s subject '%s', received time: %s, "
				"sent time: %s, modif time: %s", message.GetSenderName(), 
				message.GetSenderEmail(), recipients, message.GetSubject(), 
				Format(receivedTime), Format(submitTime), Format(modifTime)));
		puts(Format("Body: %s", message.GetBody(false)));
		//puts(Format("HTML: %s", message.GetHTML()));
		if(message.GetAttachmentCount()) {
			printf(Format("Saving attachments to %s...", MSG_ATTACHMENT_FOLDER));
			message.SaveAttachment(MSG_ATTACHMENT_FOLDER);
			puts("done");
		}
		static int num;
		String fileName = "c:\\temp\\" + FormatInt(num) + ".msg";
		printf(Format("Saving message to %s ...", fileName.Begin()));
		message.SaveToFile(fileName);
		num++;
		puts("done");
	}
}

// This function creates a folder (opens if exists) and copies the first unread message if any 
// to this folder
// You can also move and delete the message, but I wanted the sample to be non destructive just 
// in case
void CopyMessageTest(MAPIEx& mapi) {
	puts("\nCopy message test");
	MAPIFolder folder;
	if(!mapi.OpenInbox(folder))
		return; 
	//mapi.SetUnreadOnly();

	MAPIMessage message;
	if(folder.GetNextMessage(message)) {
		puts(Format("Copying message from '%s' subject '%s'", message.GetSenderName(), 
															message.GetSubject()));
		printf(Format(", from Inbox to Inbox/%s'", COPY_MSG_FOLDER));
		MAPIFolder subFolder; 
		if(folder.CreateSubFolder(COPY_MSG_FOLDER, subFolder)) 
			folder.CopyMessage(message, subFolder);
	}
}

long CALLBACK OnNewEvent(LPVOID lpvContext, ULONG cNotification, LPNOTIFICATION lpNotifications) {
	String *data = (String *)lpvContext;
	puts(Format("Received new event. Referenced data is %s.", data));
	puts("Activated event was ");
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
		puts("Waiting for a new incoming message... (hit a key to cancel)");
		while(!_kbhit()) 
			Sleep(100);
	}
}

void ContactsTest(MAPIEx& mapi) {
	puts("\nContacts test");
	MAPIFolder contacts;
	if (!mapi.OpenContacts(contacts))
		return;
	
	// sort by name (stored in PR_SUBJECT)
	contacts.SortContents(TABLE_SORT_ASCEND, PR_SUBJECT);

	MAPIContact contact;
	ContactAddress address;
	if(contacts.GetNextContact(contact)) {
		puts(Format("Name: '%s'", contact.GetName()));
		puts(Format("Birthday: %s", Format(contact.GetBirthday())));
		puts(Format("Anniversary: %s", Format(contact.GetAnniversary())));
		puts(Format("Categories: '%s'", contact.GetCategories()));
		puts(Format("Company: '%s'", contact.GetCompany()));
		puts(Format("Email: '%s'", contact.GetEmail()));
		puts(Format("HomePage: '%s'", contact.GetHomePage()));
		puts(Format("IM Address: '%s'", contact.GetIMAddress()));
		if(contact.GetAddress(address, ContactAddress::BUSINESS)) 
			puts(Format("%s\n%s\n%s\n%s\n%s",address.m_strStreet,
				address.m_strCity, address.m_strStateOrProvince,
				address.m_strCountry, address.m_strPostalCode));
		if(contact.GetAddress(address, ContactAddress::HOME)) 
			puts(Format("%s\n%s\n%s\n%s\n%s",address.m_strStreet, 
				address.m_strCity, address.m_strStateOrProvince,
				address.m_strCountry, address.m_strPostalCode));
		puts(Format("Phone: %s", contact.GetPhoneNumber(PR_BUSINESS_TELEPHONE_NUMBER)));
		puts(Format("Notes: %s", contact.GetBody()));

		if(contact.HasPicture()) {
			#define DEFAULT_BUFFER_SIZE	4096
			// shows how to read the bytes in, could save this or stream it to memory etc
			MAPIAttachment attachment;
			if(contact.GetAttachment(attachment, 0)) {
				if(attachment.OpenStream()) {
					BYTE buf[DEFAULT_BUFFER_SIZE];
					int nRead, nSize = 0;

					do {
						nRead = attachment.Read(buf, DEFAULT_BUFFER_SIZE);
						if(nRead > 0) 
							nSize += nRead;
					} while (nRead>0);
					attachment.CloseStream();
					puts(Format("Contact has picture (%d bytes)\n", nSize));
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
	puts("\nCreate Draft Test");
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
		puts("Draft sent successfully");
	else if(nResult == IDCANCEL) {
		message.Save();
		puts("Draft created successfully");
	}
}

void SelectContactsTest(MAPIEx& mapi) {
	puts("\nSelect Contacts Test");
	Vector <String> emails;
	if(mapi.SelectContacts(emails, "ShowAddressBook Test")) {
		for(int i = 0; i < emails.GetCount(); i++) 
			puts(Format("AddressBook recipient %d: %s", i+1, emails[i]));
	}
}

// This function creates a contact and sets its name and email
void CreateContactTest(MAPIEx& mapi) {
	puts("\nCreate Contact Test");
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
			puts("Contact created successfully");
	}	
}

// This function opens the first subfolder under contacts and if successful displays the 
// first contact if any
void ContactSubFolderTest(MAPIEx& mapi) {
	puts("\nContact SubFolder Test");
	MAPIFolder contacts;
	if(!mapi.OpenContacts(contacts)) 
		return;
	
	String strFolder;
	MAPIFolder folder;
	if(contacts.GetNextSubFolder(folder, strFolder)) {
		puts(Format("Contact Sub Folder: %s", strFolder));
		String strText;
		MAPIContact contact;
		ContactAddress address;
		if(folder.GetNextContact(contact)) 
			puts(Format("Contact Name '%s'", contact.GetName()));
	}
}

void AppointmentTest(MAPIEx& mapi) {
	puts("\nAppointment Test");
	MAPIFolder appointments;
	if(!mapi.OpenCalendar(appointments)) 
		return;
		
	String strText;
	MAPIAppointment appointment;
	if (appointments.GetNextAppointment(appointment)) {
		puts(Format("Subject: %s", appointment.GetSubject()));
		puts(Format("Location: %s", appointment.GetLocation()));
		puts(Format("Start: %s", Format(appointment.GetStartTime())));
		puts(Format("End: %s", Format(appointment.GetEndTime())));
	}
}

void EnumerateSubFolders(MAPIEx& mapi, MAPIFolder& folder, String parentName) {
	String subFolderName;
	MAPIFolder subFolder;
	while(folder.GetNextSubFolder(subFolder, subFolderName)) {
		String folderName = parentName + "/" + subFolderName;
		puts(Format("Folder: %s (%d elems)", folderName, subFolder.GetCount()));
		EnumerateSubFolders(mapi, subFolder, folderName);
	}
} 

void FolderList(MAPIEx& mapi) {
	MAPIFolder folder;
	if(mapi.OpenRootFolder(folder)) 
		EnumerateSubFolders(mapi, folder, "");
}


#include <mapi.h>
#include <mapix.h>
#include <mapiutil.h>


CONSOLE_APP_MAIN {
	MAPIEx mapi;
	if(!mapi.Login()) {
		puts("Failed to initialize MAPI");
		return;
	}

	puts(Format("Profile Name: %s", mapi.GetProfileName()));
	puts(Format("Profile Email: %s", mapi.GetProfileEmail()));

	int stDefault = Null;
	String stDefaultName;
	puts("\nStore and folders list:");
	int msLen = mapi.GetMessageStoreCount();
	for (int i = 0; i < msLen; ++i) {
		String name;
		bool isdefault;
		if(mapi.GetMessageStoreProperties(i, name, isdefault))
			puts(Format("%d: %s %s", i, name, isdefault ? "(default)" : ""));	
		else
			puts(Format("%d: Problem in store", i));	
		if (isdefault) {
			stDefault = i;
			stDefaultName = name;
		}
	}
	
	if(!mapi.OpenMessageStore(stDefault)) {
		puts("Failed to open store");
		return;
	}
	puts(Format("\nOpening default store %d: %s", stDefault, stDefaultName));
	FolderList(mapi);
	
	puts("\nPress a key to continue...");	getchar();
	
	// uncomment the functions you're interested in and/or step through these to see how each thing works.
	//ReadMessageTest(mapi);
	//CopyMessageTest(mapi);
	//CreateContactTest(mapi);
	ContactsTest(mapi);
	//SelectContactsTest(mapi);
	//CreateDraftTest(mapi);
	//SendTest(mapi);
	//SendCIDTest(mapi);
	//NotificationTest(mapi);
	//ContactSubFolderTest(mapi);
	//AppointmentTest(mapi);

	puts("\nPress a key to end...");		getchar();
}
