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

#include "TestMAPI.h"

// To make deploying this console app easier, I'm linking MAPIEx as a library; to use the DLL instead comment out the following lines 
// and change the dependency to MAPIEx
#undef AFX_EXT_CLASS
#define AFX_EXT_CLASS

#include "../../MAPIEx.h"
#include <conio.h>

// you may want to change these to valid settings before testing
#define FROM_NAME _T("Support")
#define FROM_EMAIL _T("support@nospam.com")
#define MSG_ATTACHMENT _T("c:\\temp\\pic.jpg")
#define MSG_PATH _T("c:\\temp\\message.msg")
#define TO_EMAIL _T("noel@nospam.com")
#define TO_EMAIL2 _T("noel2@nospam.com")
#define COPY_MSG_FOLDER _T("TestFolder")
#define PERSONAL_FOLDERS _T("Personal Folders")
#define DEFAULT_BUFFER_SIZE 4096

#ifdef UNICODE
#define PRINTF wprintf
#else
#define PRINTF printf
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To send a message:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-then open the outbox
//		-create a new message, set its priority if you like 
//		-set its properties, recipients and attachments
//		-call send
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendTest(CMAPIEx& mapi)
{
	if(mapi.OpenOutbox()) 
	{
		CMAPIMessage message;
		if(message.Create(&mapi, IMPORTANCE_LOW)) 
		{
			// setting sender on an exchange server where the user doesn't exist will cause message.Send() to fail
			message.SetSender(FROM_NAME, FROM_EMAIL);
			message.SetSubject(_T("Subject"));

			// user SetBody for ANSI text, SetRTF for HTML and Rich Text
			message.SetBody(_T("Body"));

			// here's an HTML body (uncomment to test)
			//message.SetHTML(_T("<html><body><font size=2 color=red face=Arial><span style='font-size:10.0pt;font-family:Arial;color:red'>Body</font></body></html>"));

			// here's an RTF body (uncomment to test)
			//message.SetRTF(_T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033{\\fonttbl{\\f0\\fnil\\fcharset0 Times New Roman;}}{\\colortbl ;\\red255\\green0\\blue0;}{\\*\\generator Msftedit 5.41.15.1507;}\\viewkind4\\uc1\\pard\\cf1\\lang3082\\b\\f0\\fs20 Body\\cf0\\lang1033\\b0\\par}"));

			message.AddRecipient(TO_EMAIL);
			message.AddRecipient(TO_EMAIL2);
			message.AddAttachment(MSG_ATTACHMENT);

			if(message.Send()) PRINTF(_T("Sent successfully\n"));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To embed images inside of HTML text:
//		-send a message as usual, setting its HTML text
//		-add an <IMG tag with src="cid:<contentID>"
//		-add an attachment using the same CID
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCIDTest(CMAPIEx& mapi)
{
	if(mapi.OpenOutbox()) 
	{
		CMAPIMessage message;
		if(message.Create(&mapi, IMPORTANCE_LOW)) 
		{
			message.SetSender(FROM_NAME, FROM_EMAIL);
			message.SetSubject(_T("Subject"));

			CString strCID="123456789";
			CString strImage=_T("<html><body><IMG alt=\"\" src=\"cid:");
			strImage+=strCID;
			strImage+=_T("\" border=0></body></html>");
			message.SetRTF(strImage);

			message.AddRecipient(TO_EMAIL);
			message.AddAttachment(MSG_ATTACHMENT, NULL, strCID);

			if(message.Send()) PRINTF(_T("Sent successfully\n"));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To receive a message:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-then open the inbox and get the contents table
//		-iterate through the message using GetNextMessage() (sample below gets only unread messages)
//		-save attachments (if any) using SaveAttachments() if you like
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ReceiveTest(CMAPIEx& mapi)
{
	if(mapi.OpenInbox() && mapi.GetContents()) 
	{
		// use SortContents to sort the messages, default is ascending by PR_MESSAGE_DELIVERY_TIME
		mapi.SortContents(TABLE_SORT_DESCEND);
		mapi.SetUnreadOnly();

		CMAPIMessage message;
		while(mapi.GetNextMessage(message)) 
		{
			CString strReceivedTime;
			message.GetReceivedTime(strReceivedTime);
			PRINTF(_T("Message from '%s' (%s) subject '%s', received: %s\n"), message.GetSenderName(), message.GetSenderEmail(), message.GetSubject(), strReceivedTime);

			// use message.GetBody() to get the ANSI text body
			// use message.GetRTF() to get the RTF or decoded HTML email

			PRINTF(_T("Attachments: %d\n"), message.GetAttachmentCount());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To iterate through folders:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-then open the folder and get the hierarchy table
//		-iterate through the folders using GetNextFolder() 
//		-OpenSubFolder() is a high level shortcut and will use Depth First Search to find a sub folder 
//		 also if you use this shortcut there is no need to RELEASE as it handles the folders internally
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnumerateSubFolders(CMAPIEx& mapi, CMAPIFolder& folder)
{
	if (folder.GetHierarchy())
	{
		CString strFolder;
		CMAPIFolder subFolder;
		while(folder.GetNextSubFolder(subFolder, strFolder)) 
		{
			PRINTF(_T("Folder: %s\n"), strFolder);
			EnumerateSubFolders(mapi, subFolder);
		}
	}
} 

void FolderTest(CMAPIEx& mapi)
{
	if(mapi.OpenRootFolder() && mapi.GetHierarchy()) 
	{
		CString strFolder;
		CMAPIFolder folder;
		while(mapi.GetNextSubFolder(folder, strFolder)) 
		{
			PRINTF(_T("Folder: %s\n"), strFolder);
			EnumerateSubFolders(mapi, folder);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function creates a folder (opens if exists) and copies the first unread message if any to this folder
//
// To do this:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-then open the folder (probably inbox) and get the contents table
//		-open the message you want to move
//		-create (open if exists) the folder you want to move to
//		-copy the message 
//
// You can also move and delete the message, but I wanted the sample to be non destructive just in case
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CopyMessageTest(CMAPIEx& mapi)
{
	if(mapi.OpenInbox() && mapi.GetContents()) 
	{
		mapi.SetUnreadOnly();

		CMAPIMessage message;
		if(mapi.GetNextMessage(message)) 
		{
			PRINTF(_T("Copying message from '%s' subject '%s'\n"),message.GetSenderName(),message.GetSubject());
			CMAPIFolder* pSubFolder=mapi.GetFolder()->CreateSubFolder(COPY_MSG_FOLDER);
			if(pSubFolder) 
			{
				mapi.GetFolder()->CopyMessage(message, pSubFolder);
				delete pSubFolder;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function saves the first email maessage to MSG_PATH
//
// To do this:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-then open the folder (probably Inbox) and get the contents table
//		-open the message 
//		-save the message
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaveMessageTest(CMAPIEx& mapi)
{
	if(mapi.OpenInbox() && mapi.GetContents()) 
	{
		mapi.SortContents(TABLE_SORT_DESCEND);

		CMAPIMessage message;
		if(mapi.GetNextMessage(message)) 
		{
			PRINTF(_T("Saving message '%s'\n"), message.GetSubject());
			message.SaveAsMsg(MSG_PATH);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// These functions wait for a notification of a new message
//
// To do this:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-call Notify with the callback and a context pointer (default ulEventMask is fnevNewMail)
//
// When a new mail arrives, the OnNewMessage function will be called with the context you passed in.  
// Note that since this test program is a single threaded console app I need to pump messages, usually in 
// a windows app this would be done automatically as part of the run loop.
//
// Obviously you'll have to send yourself a mail or two during this test, also note that the notification 
// will probably arrive a little while after the message arrives, depending on your exchange server's setup
// 
// You can also call Notify with a different even mask to capture say when the user deletes something etc
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

long CALLBACK OnNewMessage(LPVOID lpvContext, ULONG cNotification, LPNOTIFICATION lpNotifications)
{
	PRINTF(_T("Recieved New Message\n"));
	return 0;
}

void NotificationTest(CMAPIEx& mapi)
{
	if(mapi.Notify(OnNewMessage, NULL, fnevNewMail)) 
	{ 
		PRINTF(_T("Waiting for a new incoming message... (hit a key to cancel)\n"));
		while(!_kbhit()) Sleep(100);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function displays your first contact in your Contacts folder
//
// To do this:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-open your contacts folder 
//
// Use GetName to get the name (default DISPLAY NAME, but can be Initials, First Name etc) 
// Use GetEmail to get the email address (named property) 
// Use GetAddress to get the mailing address of CContactAddress::AddressType
// Use GetPhoneNumber supplying a phone number property (ie PR_BUSINESS_TELEPHONE_NUMBER)
// Use GetNotes to get the notes in either plain text (default) or RTF
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactsTest(CMAPIEx& mapi)
{
	if(mapi.OpenContacts() && mapi.GetContents()) 
	{
		// sort by name (stored in PR_SUBJECT)
		mapi.SortContents(TABLE_SORT_ASCEND, PR_SUBJECT);

		CString strText;
		CMAPIContact contact;
		CContactAddress address;
		if(mapi.GetNextContact(contact)) 
		{
			if(contact.GetName(strText)) PRINTF(_T("Name: '%s'\n"), strText);
			if(contact.GetBirthday(strText)) PRINTF(_T("Birthday: %s\n"), strText);
			if(contact.GetAnniversary(strText)) PRINTF(_T("Anniversary: %s\n"), strText);
			if(contact.GetCategories(strText)) PRINTF(_T("Categories: '%s'\n"), strText);
			if(contact.GetCompany(strText)) PRINTF(_T("Company: '%s'\n"), strText);
			if(contact.GetEmail(strText)) PRINTF(_T("Email: '%s'\n"), strText);
			if(contact.GetHomePage(strText)) PRINTF(_T("HomePage: '%s'\n"), strText);
			if(contact.GetIMAddress(strText)) PRINTF(_T("IM Address: '%s'\n"), strText);
			if(contact.GetAddress(address, CContactAddress::BUSINESS)) PRINTF(_T("%s\n%s\n%s\n%s\n%s\n"),address.m_strStreet,address.m_strCity,address.m_strStateOrProvince,address.m_strCountry,address.m_strPostalCode);
			if(contact.GetAddress(address, CContactAddress::HOME)) PRINTF(_T("%s\n%s\n%s\n%s\n%s\n"),address.m_strStreet,address.m_strCity,address.m_strStateOrProvince,address.m_strCountry,address.m_strPostalCode);
			if(contact.GetPhoneNumber(strText, PR_BUSINESS_TELEPHONE_NUMBER)) PRINTF(_T("Phone: %s\n"), strText);
			if(contact.GetBody(strText)) PRINTF(_T("Notes: %s\n"), strText);

			if(contact.HasPicture())
			{
				// shows how to read the bytes in, could save this or stream it to memory etc
				CMAPIAttachment attachment;
				if(contact.GetAttachment(attachment, 0))
				{
					if(attachment.OpenStream())
					{
						BYTE buf[DEFAULT_BUFFER_SIZE];
						int nRead, nSize=0;

						do 
						{
							nRead=attachment.Read(buf, DEFAULT_BUFFER_SIZE);
							if(nRead>0) nSize+=nRead;
						} 
						while (nRead>0);
						attachment.CloseStream();
						PRINTF(_T("Contact has picture (%d bytes)\n"), nSize);
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function creates a message in the drafts folder and opens it up using the default IMessage form
//
// To do this:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-open your drafts folder 
//		-set the properties of the message and ShowForm
//
// NOTE: on new messages if ShowForm returns IDOK, the message was sent, the message will not be valid anymore, 
//       if it returns IDCANCEL, the user closed the dialog and you must save the message or it will be lost
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDraftTest(CMAPIEx& mapi)
{
	if(mapi.OpenDrafts()) 
	{
		CMAPIMessage message;
		if(message.Create(&mapi, IMPORTANCE_LOW)) 
		{
			message.SetSender(FROM_NAME, FROM_EMAIL);
			message.SetSubject(_T("Subject"));
			message.AddRecipient(TO_EMAIL);

			int nResult=message.ShowForm(&mapi);
			if(nResult==IDOK) 
			{
				PRINTF(_T("Draft sent successfully\n"));
			} 
			else if(nResult==IDCANCEL) 
			{
				message.Save();
				PRINTF(_T("Draft created successfully\n"));
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function shows the address book and prints out the email addresses of the chosen recipients
//
// To do this:
//		-first open up a MAPI session and login
//		-then use ShowAddressBook to open the dialog
//		-if IDOK is returned, iterate through the email addresses returned
//
// NOTE: you can also use CMAPIMessage::AddRecipients with pAddressList as the argument on a new email
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowAddressBookTest(CMAPIEx& mapi)
{
	LPADRLIST pAddressList;
	if(mapi.ShowAddressBook(pAddressList, _T("ShowAddressBookTest"))==IDOK) 
	{
		CString strEmail;
		for(ULONG i=0;i<pAddressList->cEntries;i++) 
		{
			if(mapi.GetEmail(pAddressList->aEntries[i], strEmail)) 
			{
				PRINTF(_T("AddressBook recipient %d: %s\n"), i+1, strEmail);
			}
		}
		CMAPIEx::ReleaseAddressList(pAddressList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function creates a contact and sets its name and email
//
// To do this:
//		-first open up a MAPI session and login
//		-then open the message store you want to access (NULL is the default store)
//		-open your contacts folder 
//		-create the contact
//		-set the properties of the contact 
//		-optionally call UpdateDisplayName after filling in name fields (will set initials, File As etc)
//		-save your changes
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateContactTest(CMAPIEx& mapi)
{
	if(mapi.OpenContacts()) 
	{
		CMAPIContact contact;
		if(contact.Create(&mapi)) 
		{
			contact.SetDisplayNamePrefix(_T("Mr."));
			contact.SetName(FROM_NAME, PR_GIVEN_NAME);
			contact.UpdateDisplayName();
			contact.SetEmail(FROM_EMAIL, 1);
			if(contact.Save()) PRINTF(_T("Contact created successfully\n"));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function opens the first subfolder under contacts and if successful displays the 
// first contact if any
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactSubFolderTest(CMAPIEx& mapi)
{
	if(mapi.OpenContacts() && mapi.GetHierarchy()) 
	{
		CString strFolder;
		CMAPIFolder folder;
		if(mapi.GetNextSubFolder(folder, strFolder)) 
		{
			PRINTF(_T("Contact Sub Folder: %s\n"), strFolder);
			if(folder.GetContents()) 
			{
				CString strText;
				CMAPIContact contact;
				CContactAddress address;
				if(folder.GetNextContact(contact)) 
				{
					if(contact.GetName(strText)) PRINTF(_T("Contact Name '%s'\n"), strText);
				}
			}
		}
	}
}

void AppointmentTest(CMAPIEx& mapi)
{
	if(mapi.OpenCalendar() && mapi.GetContents()) 
	{
		CString strText;
		CMAPIAppointment appointment;
		if (mapi.GetNextAppointment(appointment))
		{
			if(appointment.GetSubject(strText))	PRINTF(_T("%s\n"), strText);
			if(appointment.GetLocation(strText)) PRINTF(_T("%s\n"), strText);
			if(appointment.GetStartTime(strText, NULL))	PRINTF(_T("%s\n"), strText);
			if(appointment.GetEndTime(strText, NULL)) PRINTF(_T("%s\n"), strText);
		}
	}
}

// this example works on unread messages, so send yourself a message and don't open it before trying this test
// If you have "autopreview" set turn it off to run this sample, you may want to run step by step as well.
void main(int argc, char* argv[])
{
	CMAPIEx mapi;
	if(!CMAPIEx::Init() || !mapi.Login() || !mapi.OpenMessageStore()) 
	{
		PRINTF(_T("Failed to initialize MAPI\n"));
		return;
	}

	// you can use this profile name as an argument for Login() when no MAPI provider is open (ie "Outlook")
	CString strText;
	if(mapi.GetProfileName(strText)) PRINTF(_T("Profile Name: %s\n"), strText);
	if(mapi.GetProfileEmail(strText)) PRINTF(_T("Profile Email: %s\n"), strText);

	int nCount = mapi.GetMessageStoreCount();
	PRINTF(_T("MessageStore Count: %d\n"), nCount);

	// uncomment the functions you're interested in and/or step through these to see how each thing works.
// 	ShowAddressBookTest(mapi);
// 	ContactsTest(mapi);
// 	CreateDraftTest(mapi);
// 	SendTest(mapi);
// 	SendCIDTest(mapi);
// 	FolderTest(mapi);
	ReceiveTest(mapi);
// 	CopyMessageTest(mapi);
// 	SaveMessageTest(mapi);
// 	NotificationTest(mapi);
// 	CreateContactTest(mapi);
// 	ContactSubFolderTest(mapi);
// 	AppointmentTest(mapi);

	mapi.Logout();
	CMAPIEx::Term();
}
