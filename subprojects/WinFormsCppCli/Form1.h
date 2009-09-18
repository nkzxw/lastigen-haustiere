#pragma once

#include "Router.hpp"

namespace WinFormsCppCli {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:



		void ToCLRString(const std::string& source, System::String^ target)
		{
			//std::string MyInfoText("Welcome User"); 
			//String^ MyInfoDisp = gcnew String(MyInfoText.c_str());		
			target = gcnew String(source.c_str());		
		}

		System::String^ ToCLRString(const std::string& source)
		{
			return gcnew String(source.c_str());		
		}


		void addRouterToList(const Router& router)
		{
			// System::Windows::Forms::
			//string s1 = marshal_to<string>( s );
			//String^ s2 = marshal_to<String^>( s1 ); 
			//marshal_to<String^>( router.bssid_ );
			//String^ temp = ToCLRString(router.bssid_);
			//ListViewItem^ listViewItem = (gcnew ListViewItem(gcnew cli::array< String^  >(4) {"", "", "", ""}, -1));
			
			cli::array<String^>^ arr =  gcnew cli::array< String^  >(4);
			arr[0] = ToCLRString(router.bssid_);
			arr[1] = ToCLRString(router.ssid_);
			//arr[2] = ToCLRString(router.signal_);
			arr[2] = router.signal_.ToString();

			if (router.security_)
			{
				arr[3] = ToCLRString("true");
			}
			else
			{
				arr[3] = ToCLRString("false");
			}

			ListViewItem^ listViewItem = (gcnew ListViewItem(arr, -1));
			this->listView1->Items->Add(listViewItem);

			//this->listView1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(2) {listViewItem1, listViewItem2});
		}


		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 




	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  bssid;
	private: System::Windows::Forms::ColumnHeader^  ssid;
	private: System::Windows::Forms::ColumnHeader^  signal;
	private: System::Windows::Forms::ColumnHeader^  security;



	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->bssid = (gcnew System::Windows::Forms::ColumnHeader());
			this->ssid = (gcnew System::Windows::Forms::ColumnHeader());
			this->signal = (gcnew System::Windows::Forms::ColumnHeader());
			this->security = (gcnew System::Windows::Forms::ColumnHeader());
			this->SuspendLayout();
			// 
			// listView1
			// 
			this->listView1->Activation = System::Windows::Forms::ItemActivation::OneClick;
			this->listView1->AutoArrange = false;
			this->listView1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->bssid, this->ssid, 
				this->signal, this->security});
			this->listView1->FullRowSelect = true;
			this->listView1->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->listView1->HideSelection = false;
			this->listView1->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->listView1->Location = System::Drawing::Point(12, 12);
			this->listView1->MultiSelect = false;
			this->listView1->Name = L"listView1";
			this->listView1->ShowGroups = false;
			this->listView1->Size = System::Drawing::Size(458, 346);
			this->listView1->TabIndex = 3;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// bssid
			// 
			this->bssid->Tag = L"123";
			this->bssid->Text = L"BSSID";
			this->bssid->Width = 150;
			// 
			// ssid
			// 
			this->ssid->Tag = L"345";
			this->ssid->Text = L"SSID";
			this->ssid->Width = 150;
			// 
			// signal
			// 
			this->signal->Text = L"Signal";
			// 
			// security
			// 
			this->security->Text = L"Security";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 370);
			this->Controls->Add(this->listView1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				//System::Windows::Forms::ListViewItem^  listViewItem1 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"hola", 
				//	L"sdfsfsdfsdf"}, -1));
				//System::Windows::Forms::ListViewItem^  listViewItem2 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"", 
				//	L"como tas"}, -1));
	 			//this->listView1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(2) {listViewItem1, listViewItem2});



				 Router tempRouter("00-00-00-00-00-00", "...", 0, 0, false);
				 addRouterToList(tempRouter);
			 }
	private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
			 {
			 }
	};
}

