#pragma once

#include "process.h"
#include <iostream>
#include <vector>
#include <algorithm>

struct Regions regions;
IplImage *imageColor;
IplImage *imageRenderColor;

#define DRAW_OPER_SELECT 1
#define DRAW_OPER_TRACK  2
#define DRAW_OPER_MOVE   4
#define DRAW_OPER_LINE   8
#define DRAW_OPER_DEPTH  16
#define DRAW_OPER_NEW_REGION 32
#define DRAW_OPER_OPTIONS	64

#define DRAW_MOVE_REGION	DRAW_OPER_SELECT  | DRAW_OPER_MOVE
#define DRAW_SCALAR			DRAW_OPER_SELECT  | DRAW_OPER_OPTIONS
#define DRAW_DEPTH_LINE		DRAW_OPER_TRACK   | DRAW_OPER_LINE    | DRAW_OPER_DEPTH
#define DRAW_LINE			DRAW_OPER_TRACK	  | DRAW_OPER_LINE
#define DRAW_NEW_REGION		DRAW_OPER_TRACK   | DRAW_OPER_NEW_REGION

int drawOperations = DRAW_LINE;

int lastPosX;
int lastPosY;
int totalDiffX;
int totalDiffY;
int lastRegion = -1;

std::vector<int> vecX;
std::vector<int> vecY;

namespace CatToons {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
    // GetCursorPos() makes everything possible
    //static extern bool GetCursorPos(POINT p);
//	static extern BOOL GetCursorPos(LPPOINT p);
    // Variable we will need to count the traveled pixels

	String ^formatLog(String ^strin)
	{
		String ^str;
		str = System::DateTime::Now.ToString("dd/MM/yyyy hh:mm:ss") + " - " + strin + "\n";
		return str;
	}

	System::Drawing::Bitmap ^ImageToBitmap(IplImage *img)
	{
		return gcnew System::Drawing::Bitmap(img->width, img->height,img->widthStep,System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr)img->imageData);
	}

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
			this->originalFilename->Empty;
			this->colorFilename->Empty;
			pictureBox1->Image = nullptr;
			pictureBox2->Image = nullptr;
			pictureBox3->Image = nullptr;
			pictureBox4->Image = nullptr;
			pictureBox5->Image = nullptr;
			pictureBox6->Image = nullptr;
			pictureBox7->Image = nullptr;
			pictureBox8->Image = nullptr;
			pictureBox9->Image = nullptr;
			pictureBox10->Image = nullptr;
			pictureBox11->Image = nullptr;
			pictureBox12->Image = nullptr;
			pictNormal2->Image = nullptr;

			currentImage = nullptr;
			regions.size = 0;
			imageColor = 0;
			imageRenderColor = 0;

			
			//
			//TODO: Add the constructor code here
			//
			StatusBar->Text = "Line In";
			lineItem->Checked = true;
		}

	protected:
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


	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Drawing::Graphics ^g;

	private: System::Windows::Forms::ToolStrip^  toolStrip1;

	private: System::Windows::Forms::ToolStripButton^  btnRun;

	private: String ^originalFilename;
	private: String ^colorFilename;
	private: String ^fileName;
	private: Image  ^currentImage;

	private: System::Windows::Forms::ToolStripButton^  btnColor;

	private: System::Windows::Forms::ToolStripButton^  btnOriginal;





	private: System::Windows::Forms::ToolStripButton^  btnSave;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::TabPage^  tabPagLog;
	private: System::Windows::Forms::RichTextBox^  txtLog;
	private: System::Windows::Forms::TabPage^  tabPagShadder;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::TabControl^  tabControl2;
	private: System::Windows::Forms::TabPage^  tabPagPhongGray;
	private: System::Windows::Forms::PictureBox^  pictureBox10;
	private: System::Windows::Forms::TabPage^  tabPagPhong;
	private: System::Windows::Forms::PictureBox^  pictureBox8;
	private: System::Windows::Forms::TabPage^  tabPagPhongRender;
	private: System::Windows::Forms::PictureBox^  pictureBox11;
	private: System::Windows::Forms::TabPage^  tabPagThreshold;
	private: System::Windows::Forms::PictureBox^  pictureBox9;
	private: System::Windows::Forms::TabPage^  tabPagThresholdRender;
	private: System::Windows::Forms::PictureBox^  pictureBox12;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  btnCalcular;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::CheckBox^  ckbFilled;
	private: System::Windows::Forms::CheckBox^  ckbWithBorder;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  numIncr;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  numPerc;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::NumericUpDown^  numSilhueta;
	private: System::Windows::Forms::CheckBox^  ckbSpecular;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::NumericUpDown^  numShine;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  numLuzZ;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  numLuzY;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  numLuzX;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  numObsZ;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  numObsY;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  numObsX;
	private: System::Windows::Forms::TabPage^  tabPagNormal2;
	private: System::Windows::Forms::PictureBox^  pictNormal2;
	private: System::Windows::Forms::TabPage^  tabPagNormal;
	private: System::Windows::Forms::PictureBox^  pictureBox7;
	private: System::Windows::Forms::TabPage^  tabPagColorImage;
	private: System::Windows::Forms::PictureBox^  pictureBox6;
	private: System::Windows::Forms::TabPage^  tabPagChaincode;
	private: System::Windows::Forms::PictureBox^  pictureBox5;
	private: System::Windows::Forms::TabPage^  tabPagFloodfill;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::TabPage^  tabPagThinning;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::TabPage^  tabPagColor;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::TabPage^  tabPagOriginal;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::TabControl^  tabControl1;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::NumericUpDown^  numSize;
private: System::Windows::Forms::TabPage^  tabPagNormShad;
private: System::Windows::Forms::PictureBox^  pictureBox13;
		 private: bool clickMouse;
private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  newRegionItem;
private: System::Windows::Forms::ToolStripMenuItem^  lineItem;


private: System::Windows::Forms::StatusStrip^  statusStrip1;
private: System::Windows::Forms::ToolStripStatusLabel^  StatusBar;
private: System::Windows::Forms::ToolStripMenuItem^  moveRegionItem;
private: System::Windows::Forms::ToolStripMenuItem^  scalarToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
private: System::Windows::Forms::ToolStripMenuItem^  Scalaritem;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem4;




private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		bool isDualFileSelected()
		{
			if(this->originalFilename == nullptr)
				return false;
			if(this->colorFilename == nullptr)
				return false;
			return true;
		}
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->btnOriginal = (gcnew System::Windows::Forms::ToolStripButton());
			this->btnColor = (gcnew System::Windows::Forms::ToolStripButton());
			this->btnRun = (gcnew System::Windows::Forms::ToolStripButton());
			this->btnSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tabPagLog = (gcnew System::Windows::Forms::TabPage());
			this->txtLog = (gcnew System::Windows::Forms::RichTextBox());
			this->tabPagShadder = (gcnew System::Windows::Forms::TabPage());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPagPhongGray = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox10 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagPhong = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagPhongRender = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox11 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagThreshold = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox9 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagThresholdRender = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox12 = (gcnew System::Windows::Forms::PictureBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->btnCalcular = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->numSize = (gcnew System::Windows::Forms::NumericUpDown());
			this->ckbFilled = (gcnew System::Windows::Forms::CheckBox());
			this->ckbWithBorder = (gcnew System::Windows::Forms::CheckBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->numIncr = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->numPerc = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->numSilhueta = (gcnew System::Windows::Forms::NumericUpDown());
			this->ckbSpecular = (gcnew System::Windows::Forms::CheckBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->numShine = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->numLuzZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->numLuzY = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->numLuzX = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->numObsZ = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->numObsY = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numObsX = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPagNormal2 = (gcnew System::Windows::Forms::TabPage());
			this->pictNormal2 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagNormal = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagColorImage = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagChaincode = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagFloodfill = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagThinning = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagColor = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPagOriginal = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPagNormShad = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox13 = (gcnew System::Windows::Forms::PictureBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->newRegionItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lineItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moveRegionItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scalarToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Scalaritem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->StatusBar = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1->SuspendLayout();
			this->tabPagLog->SuspendLayout();
			this->tabPagShadder->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tabPagPhongGray->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox10))->BeginInit();
			this->tabPagPhong->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox8))->BeginInit();
			this->tabPagPhongRender->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox11))->BeginInit();
			this->tabPagThreshold->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox9))->BeginInit();
			this->tabPagThresholdRender->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox12))->BeginInit();
			this->panel1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numSize))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numIncr))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numPerc))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numSilhueta))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numShine))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numLuzZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numLuzY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numLuzX))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numObsZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numObsY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numObsX))->BeginInit();
			this->tabPagNormal2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictNormal2))->BeginInit();
			this->tabPagNormal->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox7))->BeginInit();
			this->tabPagColorImage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox6))->BeginInit();
			this->tabPagChaincode->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->BeginInit();
			this->tabPagFloodfill->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			this->tabPagThinning->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			this->tabPagColor->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->tabPagOriginal->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPagNormShad->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox13))->BeginInit();
			this->contextMenuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->btnOriginal, this->btnColor, 
				this->btnRun, this->btnSave});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(1264, 86);
			this->toolStrip1->TabIndex = 1;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// btnOriginal
			// 
			this->btnOriginal->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnOriginal.Image")));
			this->btnOriginal->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->btnOriginal->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnOriginal->Name = L"btnOriginal";
			this->btnOriginal->Size = System::Drawing::Size(68, 83);
			this->btnOriginal->Text = L"Art Line";
			this->btnOriginal->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->btnOriginal->ToolTipText = L"Abrir ArtLine";
			this->btnOriginal->Click += gcnew System::EventHandler(this, &Form1::btnOriginal_Click);
			// 
			// btnColor
			// 
			this->btnColor->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnColor.Image")));
			this->btnColor->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->btnColor->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->btnColor->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnColor->Name = L"btnColor";
			this->btnColor->Size = System::Drawing::Size(68, 83);
			this->btnColor->Text = L"Colorida";
			this->btnColor->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->btnColor->TextImageRelation = System::Windows::Forms::TextImageRelation::Overlay;
			this->btnColor->ToolTipText = L"Abrir Imagem Colorida";
			this->btnColor->Click += gcnew System::EventHandler(this, &Form1::btnColor_Click);
			// 
			// btnRun
			// 
			this->btnRun->Enabled = false;
			this->btnRun->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnRun.Image")));
			this->btnRun->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->btnRun->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->btnRun->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnRun->Name = L"btnRun";
			this->btnRun->Size = System::Drawing::Size(68, 83);
			this->btnRun->Text = L"Processar";
			this->btnRun->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->btnRun->TextImageRelation = System::Windows::Forms::TextImageRelation::Overlay;
			this->btnRun->ToolTipText = L"Executar";
			this->btnRun->Click += gcnew System::EventHandler(this, &Form1::btnRun_Click);
			// 
			// btnSave
			// 
			this->btnSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnSave.Image")));
			this->btnSave->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->btnSave->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->btnSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnSave->Name = L"btnSave";
			this->btnSave->Size = System::Drawing::Size(68, 83);
			this->btnSave->Text = L"Salvar";
			this->btnSave->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->btnSave->TextImageRelation = System::Windows::Forms::TextImageRelation::Overlay;
			this->btnSave->ToolTipText = L"Salvar";
			this->btnSave->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
			// 
			// tabPagLog
			// 
			this->tabPagLog->Controls->Add(this->txtLog);
			this->tabPagLog->Location = System::Drawing::Point(4, 22);
			this->tabPagLog->Name = L"tabPagLog";
			this->tabPagLog->Padding = System::Windows::Forms::Padding(3);
			this->tabPagLog->Size = System::Drawing::Size(1256, 652);
			this->tabPagLog->TabIndex = 7;
			this->tabPagLog->Text = L"Log";
			this->tabPagLog->UseVisualStyleBackColor = true;
			// 
			// txtLog
			// 
			this->txtLog->Dock = System::Windows::Forms::DockStyle::Fill;
			this->txtLog->Location = System::Drawing::Point(3, 3);
			this->txtLog->Name = L"txtLog";
			this->txtLog->Size = System::Drawing::Size(1250, 646);
			this->txtLog->TabIndex = 0;
			this->txtLog->Text = L"";
			// 
			// tabPagShadder
			// 
			this->tabPagShadder->Controls->Add(this->splitContainer1);
			this->tabPagShadder->Location = System::Drawing::Point(4, 22);
			this->tabPagShadder->Name = L"tabPagShadder";
			this->tabPagShadder->Padding = System::Windows::Forms::Padding(3);
			this->tabPagShadder->Size = System::Drawing::Size(1256, 652);
			this->tabPagShadder->TabIndex = 8;
			this->tabPagShadder->Text = L"Shadder";
			this->tabPagShadder->UseVisualStyleBackColor = true;
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(3, 3);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->tabControl2);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->panel1);
			this->splitContainer1->Size = System::Drawing::Size(1250, 646);
			this->splitContainer1->SplitterDistance = 1069;
			this->splitContainer1->TabIndex = 0;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tabPagPhongGray);
			this->tabControl2->Controls->Add(this->tabPagPhong);
			this->tabControl2->Controls->Add(this->tabPagPhongRender);
			this->tabControl2->Controls->Add(this->tabPagThreshold);
			this->tabControl2->Controls->Add(this->tabPagThresholdRender);
			this->tabControl2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl2->Location = System::Drawing::Point(0, 0);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(1069, 646);
			this->tabControl2->TabIndex = 0;
			// 
			// tabPagPhongGray
			// 
			this->tabPagPhongGray->Controls->Add(this->pictureBox10);
			this->tabPagPhongGray->Location = System::Drawing::Point(4, 22);
			this->tabPagPhongGray->Name = L"tabPagPhongGray";
			this->tabPagPhongGray->Padding = System::Windows::Forms::Padding(3);
			this->tabPagPhongGray->Size = System::Drawing::Size(1061, 620);
			this->tabPagPhongGray->TabIndex = 2;
			this->tabPagPhongGray->Text = L"Phong Gray";
			this->tabPagPhongGray->UseVisualStyleBackColor = true;
			this->tabPagPhongGray->Enter += gcnew System::EventHandler(this, &Form1::tabPagPhongGray_Enter);
			// 
			// pictureBox10
			// 
			this->pictureBox10->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox10->Location = System::Drawing::Point(3, 3);
			this->pictureBox10->Name = L"pictureBox10";
			this->pictureBox10->Size = System::Drawing::Size(1055, 614);
			this->pictureBox10->TabIndex = 0;
			this->pictureBox10->TabStop = false;
			// 
			// tabPagPhong
			// 
			this->tabPagPhong->Controls->Add(this->pictureBox8);
			this->tabPagPhong->Location = System::Drawing::Point(4, 22);
			this->tabPagPhong->Name = L"tabPagPhong";
			this->tabPagPhong->Padding = System::Windows::Forms::Padding(3);
			this->tabPagPhong->Size = System::Drawing::Size(1061, 620);
			this->tabPagPhong->TabIndex = 0;
			this->tabPagPhong->Text = L"Phong Color";
			this->tabPagPhong->UseVisualStyleBackColor = true;
			this->tabPagPhong->Enter += gcnew System::EventHandler(this, &Form1::tabPagPhong_Enter);
			// 
			// pictureBox8
			// 
			this->pictureBox8->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox8->Location = System::Drawing::Point(3, 3);
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->Size = System::Drawing::Size(1055, 614);
			this->pictureBox8->TabIndex = 0;
			this->pictureBox8->TabStop = false;
			this->pictureBox8->Click += gcnew System::EventHandler(this, &Form1::pictureBox8_Click);
			// 
			// tabPagPhongRender
			// 
			this->tabPagPhongRender->Controls->Add(this->pictureBox11);
			this->tabPagPhongRender->Location = System::Drawing::Point(4, 22);
			this->tabPagPhongRender->Name = L"tabPagPhongRender";
			this->tabPagPhongRender->Padding = System::Windows::Forms::Padding(3);
			this->tabPagPhongRender->Size = System::Drawing::Size(1061, 620);
			this->tabPagPhongRender->TabIndex = 3;
			this->tabPagPhongRender->Text = L"Phong Render";
			this->tabPagPhongRender->UseVisualStyleBackColor = true;
			this->tabPagPhongRender->Enter += gcnew System::EventHandler(this, &Form1::tabPagPhongRender_Enter);
			// 
			// pictureBox11
			// 
			this->pictureBox11->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox11->Location = System::Drawing::Point(3, 3);
			this->pictureBox11->Name = L"pictureBox11";
			this->pictureBox11->Size = System::Drawing::Size(1055, 614);
			this->pictureBox11->TabIndex = 0;
			this->pictureBox11->TabStop = false;
			this->pictureBox11->Click += gcnew System::EventHandler(this, &Form1::pictureBox11_Click);
			// 
			// tabPagThreshold
			// 
			this->tabPagThreshold->Controls->Add(this->pictureBox9);
			this->tabPagThreshold->Location = System::Drawing::Point(4, 22);
			this->tabPagThreshold->Name = L"tabPagThreshold";
			this->tabPagThreshold->Padding = System::Windows::Forms::Padding(3);
			this->tabPagThreshold->Size = System::Drawing::Size(1061, 620);
			this->tabPagThreshold->TabIndex = 1;
			this->tabPagThreshold->Text = L"Threshold";
			this->tabPagThreshold->UseVisualStyleBackColor = true;
			this->tabPagThreshold->Enter += gcnew System::EventHandler(this, &Form1::tabPagThreshold_Enter);
			// 
			// pictureBox9
			// 
			this->pictureBox9->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox9->Location = System::Drawing::Point(3, 3);
			this->pictureBox9->Name = L"pictureBox9";
			this->pictureBox9->Size = System::Drawing::Size(1055, 614);
			this->pictureBox9->TabIndex = 0;
			this->pictureBox9->TabStop = false;
			this->pictureBox9->Click += gcnew System::EventHandler(this, &Form1::pictureBox9_Click);
			// 
			// tabPagThresholdRender
			// 
			this->tabPagThresholdRender->Controls->Add(this->pictureBox12);
			this->tabPagThresholdRender->Location = System::Drawing::Point(4, 22);
			this->tabPagThresholdRender->Name = L"tabPagThresholdRender";
			this->tabPagThresholdRender->Padding = System::Windows::Forms::Padding(3);
			this->tabPagThresholdRender->Size = System::Drawing::Size(1061, 620);
			this->tabPagThresholdRender->TabIndex = 4;
			this->tabPagThresholdRender->Text = L"Threshold Render";
			this->tabPagThresholdRender->UseVisualStyleBackColor = true;
			this->tabPagThresholdRender->Enter += gcnew System::EventHandler(this, &Form1::tabPagThresholdRender_Enter);
			// 
			// pictureBox12
			// 
			this->pictureBox12->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox12->Location = System::Drawing::Point(3, 3);
			this->pictureBox12->Name = L"pictureBox12";
			this->pictureBox12->Size = System::Drawing::Size(1055, 614);
			this->pictureBox12->TabIndex = 2;
			this->pictureBox12->TabStop = false;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->btnCalcular);
			this->panel1->Controls->Add(this->groupBox3);
			this->panel1->Controls->Add(this->groupBox2);
			this->panel1->Controls->Add(this->groupBox1);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(177, 646);
			this->panel1->TabIndex = 0;
			// 
			// btnCalcular
			// 
			this->btnCalcular->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnCalcular->AutoSize = true;
			this->btnCalcular->Location = System::Drawing::Point(49, 521);
			this->btnCalcular->Name = L"btnCalcular";
			this->btnCalcular->Size = System::Drawing::Size(75, 33);
			this->btnCalcular->TabIndex = 3;
			this->btnCalcular->Text = L"Calcular";
			this->btnCalcular->UseVisualStyleBackColor = true;
			this->btnCalcular->Click += gcnew System::EventHandler(this, &Form1::btnCalcular_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox3->AutoSize = true;
			this->groupBox3->Controls->Add(this->label11);
			this->groupBox3->Controls->Add(this->numSize);
			this->groupBox3->Controls->Add(this->ckbFilled);
			this->groupBox3->Controls->Add(this->ckbWithBorder);
			this->groupBox3->Controls->Add(this->label8);
			this->groupBox3->Controls->Add(this->numIncr);
			this->groupBox3->Controls->Add(this->label7);
			this->groupBox3->Controls->Add(this->numPerc);
			this->groupBox3->Location = System::Drawing::Point(11, 347);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(151, 164);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Hatching";
			this->groupBox3->Enter += gcnew System::EventHandler(this, &Form1::groupBox3_Enter);
			// 
			// label11
			// 
			this->label11->Location = System::Drawing::Point(13, 84);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(48, 13);
			this->label11->TabIndex = 15;
			this->label11->Text = L"Size";
			// 
			// numSize
			// 
			this->numSize->Location = System::Drawing::Point(66, 80);
			this->numSize->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->numSize->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numSize->Name = L"numSize";
			this->numSize->Size = System::Drawing::Size(63, 20);
			this->numSize->TabIndex = 14;
			this->numSize->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			// 
			// ckbFilled
			// 
			this->ckbFilled->AutoSize = true;
			this->ckbFilled->CheckAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->ckbFilled->Checked = true;
			this->ckbFilled->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckbFilled->Location = System::Drawing::Point(9, 128);
			this->ckbFilled->Name = L"ckbFilled";
			this->ckbFilled->Size = System::Drawing::Size(50, 17);
			this->ckbFilled->TabIndex = 13;
			this->ckbFilled->Text = L"Filled";
			this->ckbFilled->UseVisualStyleBackColor = true;
			// 
			// ckbWithBorder
			// 
			this->ckbWithBorder->AutoSize = true;
			this->ckbWithBorder->CheckAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->ckbWithBorder->Checked = true;
			this->ckbWithBorder->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckbWithBorder->Location = System::Drawing::Point(9, 105);
			this->ckbWithBorder->Name = L"ckbWithBorder";
			this->ckbWithBorder->Size = System::Drawing::Size(69, 17);
			this->ckbWithBorder->TabIndex = 12;
			this->ckbWithBorder->Text = L"Bordered";
			this->ckbWithBorder->UseVisualStyleBackColor = true;
			// 
			// label8
			// 
			this->label8->Location = System::Drawing::Point(12, 58);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(48, 13);
			this->label8->TabIndex = 11;
			this->label8->Text = L"Increase";
			// 
			// numIncr
			// 
			this->numIncr->Location = System::Drawing::Point(65, 54);
			this->numIncr->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->numIncr->Name = L"numIncr";
			this->numIncr->Size = System::Drawing::Size(63, 20);
			this->numIncr->TabIndex = 10;
			this->numIncr->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {70, 0, 0, 0});
			// 
			// label7
			// 
			this->label7->Location = System::Drawing::Point(12, 29);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(48, 13);
			this->label7->TabIndex = 9;
			this->label7->Text = L"Percentil";
			// 
			// numPerc
			// 
			this->numPerc->DecimalPlaces = 2;
			this->numPerc->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->numPerc->Location = System::Drawing::Point(65, 25);
			this->numPerc->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numPerc->Name = L"numPerc";
			this->numPerc->Size = System::Drawing::Size(63, 20);
			this->numPerc->TabIndex = 8;
			this->numPerc->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox2->AutoSize = true;
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Controls->Add(this->numSilhueta);
			this->groupBox2->Controls->Add(this->ckbSpecular);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->numShine);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->numLuzZ);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->numLuzY);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->numLuzX);
			this->groupBox2->Location = System::Drawing::Point(11, 145);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(151, 188);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Luz";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &Form1::groupBox2_Enter);
			// 
			// label10
			// 
			this->label10->Location = System::Drawing::Point(6, 151);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(54, 18);
			this->label10->TabIndex = 16;
			this->label10->Text = L"Ambiente";
			// 
			// numSilhueta
			// 
			this->numSilhueta->DecimalPlaces = 2;
			this->numSilhueta->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->numSilhueta->Location = System::Drawing::Point(65, 149);
			this->numSilhueta->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numSilhueta->Name = L"numSilhueta";
			this->numSilhueta->Size = System::Drawing::Size(63, 20);
			this->numSilhueta->TabIndex = 15;
			this->numSilhueta->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 65536});
			// 
			// ckbSpecular
			// 
			this->ckbSpecular->AutoSize = true;
			this->ckbSpecular->CheckAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->ckbSpecular->Checked = true;
			this->ckbSpecular->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ckbSpecular->Location = System::Drawing::Point(16, 126);
			this->ckbSpecular->Name = L"ckbSpecular";
			this->ckbSpecular->Size = System::Drawing::Size(68, 17);
			this->ckbSpecular->TabIndex = 14;
			this->ckbSpecular->Text = L"Specular";
			this->ckbSpecular->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->ckbSpecular->UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this->label9->Location = System::Drawing::Point(13, 102);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(34, 13);
			this->label9->TabIndex = 13;
			this->label9->Text = L"Shine";
			// 
			// numShine
			// 
			this->numShine->Location = System::Drawing::Point(66, 100);
			this->numShine->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->numShine->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, System::Int32::MinValue});
			this->numShine->Name = L"numShine";
			this->numShine->Size = System::Drawing::Size(63, 20);
			this->numShine->TabIndex = 12;
			this->numShine->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 0});
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(13, 77);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(14, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Z";
			// 
			// numLuzZ
			// 
			this->numLuzZ->Location = System::Drawing::Point(66, 73);
			this->numLuzZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {50000, 0, 0, 0});
			this->numLuzZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {50000, 0, 0, System::Int32::MinValue});
			this->numLuzZ->Name = L"numLuzZ";
			this->numLuzZ->Size = System::Drawing::Size(63, 20);
			this->numLuzZ->TabIndex = 10;
			this->numLuzZ->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			// 
			// label5
			// 
			this->label5->Location = System::Drawing::Point(13, 51);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(14, 13);
			this->label5->TabIndex = 9;
			this->label5->Text = L"Y";
			// 
			// numLuzY
			// 
			this->numLuzY->Location = System::Drawing::Point(66, 47);
			this->numLuzY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, 0});
			this->numLuzY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, System::Int32::MinValue});
			this->numLuzY->Name = L"numLuzY";
			this->numLuzY->Size = System::Drawing::Size(63, 20);
			this->numLuzY->TabIndex = 8;
			// 
			// label6
			// 
			this->label6->Location = System::Drawing::Point(13, 25);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(14, 13);
			this->label6->TabIndex = 7;
			this->label6->Text = L"X";
			// 
			// numLuzX
			// 
			this->numLuzX->Location = System::Drawing::Point(66, 21);
			this->numLuzX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, 0});
			this->numLuzX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, System::Int32::MinValue});
			this->numLuzX->Name = L"numLuzX";
			this->numLuzX->Size = System::Drawing::Size(63, 20);
			this->numLuzX->TabIndex = 6;
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox1->AutoSize = true;
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->numObsZ);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->numObsY);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->numObsX);
			this->groupBox1->Location = System::Drawing::Point(11, 22);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(151, 110);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Observador";
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(12, 75);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(14, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Z";
			// 
			// numObsZ
			// 
			this->numObsZ->Location = System::Drawing::Point(65, 71);
			this->numObsZ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {50000, 0, 0, 0});
			this->numObsZ->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {50000, 0, 0, System::Int32::MinValue});
			this->numObsZ->Name = L"numObsZ";
			this->numObsZ->Size = System::Drawing::Size(59, 20);
			this->numObsZ->TabIndex = 4;
			this->numObsZ->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(12, 49);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(14, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Y";
			// 
			// numObsY
			// 
			this->numObsY->Location = System::Drawing::Point(65, 45);
			this->numObsY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, 0});
			this->numObsY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, System::Int32::MinValue});
			this->numObsY->Name = L"numObsY";
			this->numObsY->Size = System::Drawing::Size(59, 20);
			this->numObsY->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(12, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(14, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"X";
			// 
			// numObsX
			// 
			this->numObsX->Location = System::Drawing::Point(65, 19);
			this->numObsX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, 0});
			this->numObsX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5000, 0, 0, System::Int32::MinValue});
			this->numObsX->Name = L"numObsX";
			this->numObsX->Size = System::Drawing::Size(59, 20);
			this->numObsX->TabIndex = 0;
			// 
			// tabPagNormal2
			// 
			this->tabPagNormal2->Controls->Add(this->pictNormal2);
			this->tabPagNormal2->Location = System::Drawing::Point(4, 22);
			this->tabPagNormal2->Name = L"tabPagNormal2";
			this->tabPagNormal2->Padding = System::Windows::Forms::Padding(3);
			this->tabPagNormal2->Size = System::Drawing::Size(1256, 652);
			this->tabPagNormal2->TabIndex = 9;
			this->tabPagNormal2->Text = L"Normal2";
			this->tabPagNormal2->UseVisualStyleBackColor = true;
			this->tabPagNormal2->Enter += gcnew System::EventHandler(this, &Form1::tabPagNormal2_Enter);
			// 
			// pictNormal2
			// 
			this->pictNormal2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictNormal2->Location = System::Drawing::Point(3, 3);
			this->pictNormal2->Name = L"pictNormal2";
			this->pictNormal2->Size = System::Drawing::Size(1250, 646);
			this->pictNormal2->TabIndex = 0;
			this->pictNormal2->TabStop = false;
			// 
			// tabPagNormal
			// 
			this->tabPagNormal->Controls->Add(this->pictureBox7);
			this->tabPagNormal->Location = System::Drawing::Point(4, 22);
			this->tabPagNormal->Name = L"tabPagNormal";
			this->tabPagNormal->Padding = System::Windows::Forms::Padding(3);
			this->tabPagNormal->Size = System::Drawing::Size(1256, 652);
			this->tabPagNormal->TabIndex = 6;
			this->tabPagNormal->Text = L"Normal";
			this->tabPagNormal->UseVisualStyleBackColor = true;
			this->tabPagNormal->Enter += gcnew System::EventHandler(this, &Form1::tabPagNormal_Enter);
			// 
			// pictureBox7
			// 
			this->pictureBox7->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox7->Location = System::Drawing::Point(3, 3);
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->Size = System::Drawing::Size(1250, 646);
			this->pictureBox7->TabIndex = 0;
			this->pictureBox7->TabStop = false;
			// 
			// tabPagColorImage
			// 
			this->tabPagColorImage->Controls->Add(this->pictureBox6);
			this->tabPagColorImage->Location = System::Drawing::Point(4, 22);
			this->tabPagColorImage->Name = L"tabPagColorImage";
			this->tabPagColorImage->Padding = System::Windows::Forms::Padding(3);
			this->tabPagColorImage->Size = System::Drawing::Size(1256, 652);
			this->tabPagColorImage->TabIndex = 5;
			this->tabPagColorImage->Text = L"ColorImage";
			this->tabPagColorImage->UseVisualStyleBackColor = true;
			this->tabPagColorImage->Enter += gcnew System::EventHandler(this, &Form1::tabPagColorImage_Enter);
			// 
			// pictureBox6
			// 
			this->pictureBox6->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox6->Location = System::Drawing::Point(3, 3);
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->Size = System::Drawing::Size(1250, 646);
			this->pictureBox6->TabIndex = 0;
			this->pictureBox6->TabStop = false;
			// 
			// tabPagChaincode
			// 
			this->tabPagChaincode->Controls->Add(this->pictureBox5);
			this->tabPagChaincode->Location = System::Drawing::Point(4, 22);
			this->tabPagChaincode->Name = L"tabPagChaincode";
			this->tabPagChaincode->Padding = System::Windows::Forms::Padding(3);
			this->tabPagChaincode->Size = System::Drawing::Size(1256, 652);
			this->tabPagChaincode->TabIndex = 4;
			this->tabPagChaincode->Text = L"Chaincode";
			this->tabPagChaincode->UseVisualStyleBackColor = true;
			this->tabPagChaincode->Enter += gcnew System::EventHandler(this, &Form1::tabPagChaincode_Enter);
			// 
			// pictureBox5
			// 
			this->pictureBox5->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox5->Location = System::Drawing::Point(3, 3);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(1250, 646);
			this->pictureBox5->TabIndex = 0;
			this->pictureBox5->TabStop = false;
			// 
			// tabPagFloodfill
			// 
			this->tabPagFloodfill->AutoScroll = true;
			this->tabPagFloodfill->Controls->Add(this->pictureBox4);
			this->tabPagFloodfill->Location = System::Drawing::Point(4, 22);
			this->tabPagFloodfill->Name = L"tabPagFloodfill";
			this->tabPagFloodfill->Padding = System::Windows::Forms::Padding(3);
			this->tabPagFloodfill->Size = System::Drawing::Size(1256, 652);
			this->tabPagFloodfill->TabIndex = 3;
			this->tabPagFloodfill->Text = L"Floodfill";
			this->tabPagFloodfill->UseVisualStyleBackColor = true;
			this->tabPagFloodfill->Enter += gcnew System::EventHandler(this, &Form1::tabPagFloodfill_Enter);
			// 
			// pictureBox4
			// 
			this->pictureBox4->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox4->Location = System::Drawing::Point(3, 3);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(1250, 646);
			this->pictureBox4->TabIndex = 0;
			this->pictureBox4->TabStop = false;
			// 
			// tabPagThinning
			// 
			this->tabPagThinning->AutoScroll = true;
			this->tabPagThinning->Controls->Add(this->pictureBox3);
			this->tabPagThinning->Location = System::Drawing::Point(4, 22);
			this->tabPagThinning->Name = L"tabPagThinning";
			this->tabPagThinning->Padding = System::Windows::Forms::Padding(3);
			this->tabPagThinning->Size = System::Drawing::Size(1256, 652);
			this->tabPagThinning->TabIndex = 2;
			this->tabPagThinning->Text = L"Thinning";
			this->tabPagThinning->UseVisualStyleBackColor = true;
			this->tabPagThinning->Enter += gcnew System::EventHandler(this, &Form1::tabPagThinning_Enter);
			// 
			// pictureBox3
			// 
			this->pictureBox3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox3->Location = System::Drawing::Point(3, 3);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(1250, 646);
			this->pictureBox3->TabIndex = 0;
			this->pictureBox3->TabStop = false;
			// 
			// tabPagColor
			// 
			this->tabPagColor->AutoScroll = true;
			this->tabPagColor->Controls->Add(this->pictureBox2);
			this->tabPagColor->Location = System::Drawing::Point(4, 22);
			this->tabPagColor->Name = L"tabPagColor";
			this->tabPagColor->Padding = System::Windows::Forms::Padding(3);
			this->tabPagColor->Size = System::Drawing::Size(1256, 652);
			this->tabPagColor->TabIndex = 1;
			this->tabPagColor->Text = L"Color";
			this->tabPagColor->UseVisualStyleBackColor = true;
			this->tabPagColor->Enter += gcnew System::EventHandler(this, &Form1::tabPagColor_Enter);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox2->Location = System::Drawing::Point(3, 3);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(1250, 646);
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// tabPagOriginal
			// 
			this->tabPagOriginal->AutoScroll = true;
			this->tabPagOriginal->Controls->Add(this->pictureBox1);
			this->tabPagOriginal->Location = System::Drawing::Point(4, 22);
			this->tabPagOriginal->Name = L"tabPagOriginal";
			this->tabPagOriginal->Padding = System::Windows::Forms::Padding(3);
			this->tabPagOriginal->Size = System::Drawing::Size(1256, 652);
			this->tabPagOriginal->TabIndex = 0;
			this->tabPagOriginal->Text = L"Original";
			this->tabPagOriginal->UseVisualStyleBackColor = true;
			this->tabPagOriginal->Enter += gcnew System::EventHandler(this, &Form1::tabPagOriginal_Enter);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(3, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1250, 646);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPagOriginal);
			this->tabControl1->Controls->Add(this->tabPagColor);
			this->tabControl1->Controls->Add(this->tabPagThinning);
			this->tabControl1->Controls->Add(this->tabPagFloodfill);
			this->tabControl1->Controls->Add(this->tabPagChaincode);
			this->tabControl1->Controls->Add(this->tabPagColorImage);
			this->tabControl1->Controls->Add(this->tabPagNormal);
			this->tabControl1->Controls->Add(this->tabPagNormal2);
			this->tabControl1->Controls->Add(this->tabPagShadder);
			this->tabControl1->Controls->Add(this->tabPagNormShad);
			this->tabControl1->Controls->Add(this->tabPagLog);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 86);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1264, 678);
			this->tabControl1->TabIndex = 2;
			// 
			// tabPagNormShad
			// 
			this->tabPagNormShad->Controls->Add(this->pictureBox13);
			this->tabPagNormShad->Location = System::Drawing::Point(4, 22);
			this->tabPagNormShad->Name = L"tabPagNormShad";
			this->tabPagNormShad->Padding = System::Windows::Forms::Padding(3);
			this->tabPagNormShad->Size = System::Drawing::Size(1256, 652);
			this->tabPagNormShad->TabIndex = 10;
			this->tabPagNormShad->Text = L"Normal Shadder";
			this->tabPagNormShad->UseVisualStyleBackColor = true;
			// 
			// pictureBox13
			// 
			this->pictureBox13->BackColor = System::Drawing::Color::Black;
			this->pictureBox13->ContextMenuStrip = this->contextMenuStrip1;
			this->pictureBox13->Cursor = System::Windows::Forms::Cursors::Cross;
			this->pictureBox13->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox13->Location = System::Drawing::Point(3, 3);
			this->pictureBox13->Name = L"pictureBox13";
			this->pictureBox13->Size = System::Drawing::Size(1250, 646);
			this->pictureBox13->TabIndex = 0;
			this->pictureBox13->TabStop = false;
			this->pictureBox13->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox13_MouseMove);
			this->pictureBox13->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox13_MouseDown);
			this->pictureBox13->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox13_MouseUp);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->newRegionItem, 
				this->lineItem, this->moveRegionItem, this->scalarToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(153, 114);
			this->contextMenuStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Form1::contextMenuStrip1_ItemClicked);
			// 
			// newRegionItem
			// 
			this->newRegionItem->Name = L"newRegionItem";
			this->newRegionItem->Size = System::Drawing::Size(152, 22);
			this->newRegionItem->Text = L"New Regions";
			this->newRegionItem->Click += gcnew System::EventHandler(this, &Form1::newRegionsToolStripMenuItem_Click);
			// 
			// lineItem
			// 
			this->lineItem->Name = L"lineItem";
			this->lineItem->Size = System::Drawing::Size(152, 22);
			this->lineItem->Text = L"Line";
			this->lineItem->Click += gcnew System::EventHandler(this, &Form1::lineToolStripMenuItem_Click);
			// 
			// moveRegionItem
			// 
			this->moveRegionItem->Name = L"moveRegionItem";
			this->moveRegionItem->Size = System::Drawing::Size(152, 22);
			this->moveRegionItem->Text = L"Move Region";
			this->moveRegionItem->Click += gcnew System::EventHandler(this, &Form1::moveRegionItem_Click);
			// 
			// scalarToolStripMenuItem
			// 
			this->scalarToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->toolStripMenuItem2, 
				this->Scalaritem, this->toolStripMenuItem3, this->toolStripMenuItem4});
			this->scalarToolStripMenuItem->Name = L"scalarToolStripMenuItem";
			this->scalarToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->scalarToolStripMenuItem->Text = L"Scalar";
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(152, 22);
			this->toolStripMenuItem2->Text = L"0";
			this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &Form1::toolStripMenuItem2_Click);
			// 
			// Scalaritem
			// 
			this->Scalaritem->Name = L"Scalaritem";
			this->Scalaritem->Size = System::Drawing::Size(152, 22);
			this->Scalaritem->Text = L"1";
			this->Scalaritem->ToolTipText = L"Inserir valor";
			this->Scalaritem->Click += gcnew System::EventHandler(this, &Form1::Scalaritem_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(152, 22);
			this->toolStripMenuItem3->Text = L"10";
			this->toolStripMenuItem3->Click += gcnew System::EventHandler(this, &Form1::toolStripMenuItem3_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->StatusBar});
			this->statusStrip1->Location = System::Drawing::Point(0, 742);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1264, 22);
			this->statusStrip1->TabIndex = 3;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// StatusBar
			// 
			this->StatusBar->Name = L"StatusBar";
			this->StatusBar->Size = System::Drawing::Size(0, 17);
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(152, 22);
			this->toolStripMenuItem4->Text = L"100";
			this->toolStripMenuItem4->Click += gcnew System::EventHandler(this, &Form1::toolStripMenuItem4_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 764);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->toolStrip1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Cat Toon";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->tabPagLog->ResumeLayout(false);
			this->tabPagShadder->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->tabControl2->ResumeLayout(false);
			this->tabPagPhongGray->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox10))->EndInit();
			this->tabPagPhong->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox8))->EndInit();
			this->tabPagPhongRender->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox11))->EndInit();
			this->tabPagThreshold->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox9))->EndInit();
			this->tabPagThresholdRender->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox12))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numSize))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numIncr))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numPerc))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numSilhueta))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numShine))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numLuzZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numLuzY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numLuzX))->EndInit();
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numObsZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numObsY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numObsX))->EndInit();
			this->tabPagNormal2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictNormal2))->EndInit();
			this->tabPagNormal->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox7))->EndInit();
			this->tabPagColorImage->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox6))->EndInit();
			this->tabPagChaincode->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->EndInit();
			this->tabPagFloodfill->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			this->tabPagThinning->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			this->tabPagColor->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->tabPagOriginal->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPagNormShad->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox13))->EndInit();
			this->contextMenuStrip1->ResumeLayout(false);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void btnOriginal_Click(System::Object^  sender, System::EventArgs^  e)
{
	String ^fileName;
	openFileDialog1->Title = "Selecione imagem para tratamento";
	openFileDialog1->Filter = "All images(*.jpg,*.png,*.bmp)|*.jpg;*.png;*.bmp|JPEG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|All files (*.*)|*.*";
	openFileDialog1->FileName = "";
	openFileDialog1->Multiselect = false;
	if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
		if(pictureBox1->Image != nullptr)
		{
			delete pictureBox1->Image;
			pictureBox1->Image = nullptr;
		}
		pictureBox1->Image = System::Drawing::Image::FromFile(openFileDialog1->FileName);
	    currentImage = pictureBox1->Image;
		pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
		this->originalFilename = openFileDialog1->FileName;
		tabPagOriginal->Show();
		tabPagOriginal->Enabled = true;
		tabPagOriginal->Visible = true;
		fileName = openFileDialog1->FileName;
		int posBar = fileName->LastIndexOf('\\');
		if(posBar == 0)
		{
			posBar = fileName->LastIndexOf('/');
		}
		fileName = fileName->Substring(posBar+1);
		this->fileName = fileName;
	}

	tabControl1->SelectedTab = tabPagOriginal;
	btnRun->Enabled = isDualFileSelected();
	if(fileName == nullptr)
	{
		this->Text = "CatToons";
	}
	else
	{
		this->Text = "CatToons - " + fileName;
	}
}

private: System::Void btnColor_Click(System::Object^  sender, System::EventArgs^  e) {
	 openFileDialog1->Title = "Selecione a imagem colorida para tratamento";
	 openFileDialog1->Filter = "All images(*.jpg,*.png,*.bmp)|*.jpg;*.png;*.bmp|JPEG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|All files (*.*)|*.*";
	 openFileDialog1->FileName = "";
	 openFileDialog1->Multiselect = false;
	 if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	 {
		pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
		if(pictureBox2->Image != nullptr)
		{
			delete pictureBox2->Image;
			pictureBox2->Image = nullptr;
		}
		pictureBox2->Image = System::Drawing::Image::FromFile(openFileDialog1->FileName);
	    currentImage = pictureBox2->Image;
		pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
		this->colorFilename = openFileDialog1->FileName;
		tabPagColor->Show();
		tabPagColor->Enabled = true;
		tabPagColor->Visible = true;
	 }

	btnRun->Enabled = isDualFileSelected();
	tabControl1->SelectedTab = tabPagColor;
}
private: System::Void btnRun_Click(System::Object^  sender, System::EventArgs^  e) {
	char *strOriginal;
	char *strColor;
	IplImage *imgThinning;
	IplImage *imgFloodfill;
	IplImage *imgChaincode;
	IplImage *imgMakecolor;
	IplImage *imgNormal;
	IplImage *imgOriginal;

	if(this->originalFilename == nullptr)
	{
		MessageBox::Show("Selecione o arquivo original.");
		return;
	}
	if(this->colorFilename == nullptr)
	{
		MessageBox::Show("Selecione o arquivo colorido");
		return;
	}

	//MessageBox::Show(L"X = " + p->x + " Y = " + p->y);

	strOriginal = (char*)(void*)Marshal::StringToHGlobalAnsi(this->originalFilename);
	strColor = (char*)(void*)Marshal::StringToHGlobalAnsi(this->colorFilename);

	if(! compareImageSize(strOriginal, strColor))
	{
		MessageBox::Show("As imagens selecionadas não possuem o mesmo tamanho !!!!");
		return;
	}

	for(int i = 0; i < regions.size;i++)
	{
		regions.regions[i].branch_arr_x.clear();
		regions.regions[i].branch_arr_y.clear();
		regions.regions[i].isolated_arr_x.clear();
		regions.regions[i].isolated_arr_y.clear();
		regions.regions[i].contour_arr_x.clear();
		regions.regions[i].contour_arr_y.clear();
		regions.regions[i].fill_arr_x.clear();
		regions.regions[i].fill_arr_y.clear();
		regions.regions[i].normal.contour_arr_x.clear();
		regions.regions[i].normal.contour_arr_y.clear();
		regions.regions[i].normal.contour_norm_x.clear();
		regions.regions[i].normal.contour_norm_y.clear();
		regions.regions[i].normal.fill_arr_x.clear();
		regions.regions[i].normal.fill_arr_y.clear();
		regions.regions[i].normal.fill_norm_x.clear();
		regions.regions[i].normal.fill_norm_y.clear();
		regions.regions[i].normal.fill_norm_z.clear();
	}
	for(int i = 0; i < mapindex.size(); i++)
	{
		mapindex[i].clear();
	}
	mapindex.clear();
	regions.regions.clear();
	regions.size = 0;
	
	txtLog->Text += formatLog("Inicio Run " + fileName + " ... ");
	
	txtLog->Text += formatLog("Iniciando Thinning ");
	imgThinning = processThinning(strOriginal);
	txtLog->Text += formatLog("Finalizando Thinning ");

	numLuzX->Value = System::Decimal(-1000);
	numLuzY->Value = System::Decimal(-1000);

	numObsX->Value = System::Decimal(imgThinning->width/2);
	numObsY->Value = System::Decimal(imgThinning->height/2);

	pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	pictureBox3->Image = ImageToBitmap(imgThinning);
	currentImage = pictureBox3->Image;
	pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	tabPagThinning->Show();
	tabPagThinning->Enabled = true;
	tabPagThinning->Visible = true;
	tabControl1->SelectedTab = tabPagThinning;

	txtLog->Text += formatLog("Iniciando Floodfill ");
	imgFloodfill = processFloodFill(imgThinning, &regions);
	txtLog->Text += formatLog("Finalizando Floodfill ");
	pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	pictureBox4->Image = ImageToBitmap(imgFloodfill);
	currentImage = pictureBox4->Image;
	pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	tabPagFloodfill->Show();
	tabPagFloodfill->Enabled = true;
	tabPagFloodfill->Visible = true;
	tabControl1->SelectedTab = tabPagFloodfill;

	txtLog->Text += formatLog("Iniciando ChainCode ");
	imgChaincode = processChaincode(imgFloodfill, &regions);
	txtLog->Text += formatLog("Finalizando ChainCode ");
	pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	pictureBox5->Image = ImageToBitmap(imgChaincode);
	currentImage = pictureBox5->Image;
	pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	tabPagChaincode->Show();
	tabPagChaincode->Enabled = true;
	tabPagChaincode->Visible = true;
	tabControl1->SelectedTab = tabPagChaincode;

	txtLog->Text += formatLog("Iniciando MakeColor ");
	imgMakecolor = processMakeColor(strColor, &regions);
	txtLog->Text += formatLog("Finalizando MakeColor ");
	pictureBox6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	pictureBox6->Image = ImageToBitmap(imgMakecolor);
	currentImage = pictureBox6->Image;
	pictureBox6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	tabPagColorImage->Show();
	tabPagColorImage->Enabled = true;
	tabPagColorImage->Visible = true;
	tabControl1->SelectedTab = tabPagColorImage;

	txtLog->Text += formatLog("Iniciando Jordan Theoream ");
	processJordanTheorem(imgMakecolor, &regions);
	txtLog->Text += formatLog("Finalizando Jordan Theoream ");

	txtLog->Text += formatLog("Iniciando Normalize ");
	imgNormal = processNormalize(imgThinning, &regions);
	txtLog->Text += formatLog("Finalizando Normalize ");

	pictureBox7->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	pictureBox7->Image = ImageToBitmap(imgNormal);
	currentImage = pictureBox7->Image;
	pictureBox7->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	tabPagNormal->Show();
	tabPagNormal->Enabled = true;
	tabPagNormal->Visible = true;
	tabControl1->SelectedTab = tabPagNormal;

	txtLog->Text += formatLog("Iniciando Normalize2 ");
	IplImage *imgNormal2 = processNormalizeImage(imgFloodfill, &regions);
	txtLog->Text += formatLog("Finalizando Normalize2 ");

	pictNormal2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	pictNormal2->Image = ImageToBitmap(imgNormal2);
	currentImage = pictNormal2->Image;
	pictNormal2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	tabPagNormal2->Show();
	tabPagNormal2->Enabled = true;
	tabPagNormal2->Visible = true;
	tabControl1->SelectedTab = tabPagNormal2;


	imageColor = processColor(strColor);
	imageRenderColor = imgMakecolor;

	Marshal::FreeHGlobal((System::IntPtr)strOriginal);
	Marshal::FreeHGlobal((System::IntPtr)strColor);
	pictureBox13->Image = ImageToBitmap(imgNormal);

	for(int i = 0; i < mapindex.size(); i++)
	{
		for(int j = 0; j < mapindex[i].size(); j++)
		{
			sort(mapindex[i][j].begin(), mapindex[i][j].end());
			unique(mapindex[i][j].begin(), mapindex[i][j].end());
		}
	}

}
private: System::Void tabPagOriginal_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox1->Image;
		 }
private: System::Void tabPagColor_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox2->Image;
		 }
private: System::Void tabPagThinning_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox3->Image;
		 }
private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
	saveFileDialog1->Title = "Salvar imagem ...";
	saveFileDialog1->Filter = "All images(*.jpg,*.png,*.bmp)|*.jpg;*.png;*.bmp|JPEG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|All files (*.*)|*.*";
	saveFileDialog1->FileName = this->fileName;
	if(currentImage != nullptr)
	{
		if(saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			currentImage->Save(saveFileDialog1->FileName);
		}
	}
	else
	{
		MessageBox::Show("Não há imagem para ser salva!!!","Problema");
	}
}
private: System::Void tabPagFloodfill_Enter(System::Object^  sender, System::EventArgs^  e) {
			currentImage = pictureBox4->Image;
		 }
private: System::Void tabPagChaincode_Enter(System::Object^  sender, System::EventArgs^  e) {
			currentImage = pictureBox5->Image;
		 }
private: System::Void tabPagColorImage_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox6->Image;
		 }
private: System::Void tabPagNormal_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox7->Image;
		 }
private: System::Void groupBox2_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void btnCalcular_Click(System::Object^  sender, System::EventArgs^  e) {

		 IplImage *imgOriginal;
		 char *strOriginal;
			if(regions.size == 0)
			{
				 MessageBox::Show("Imagem ainda não foi preprocessada !!!!");
				 return;
			}

			if(this->originalFilename == nullptr)
			{
				MessageBox::Show("Selecione o arquivo original.");
				return;
			}


			int xLight = System::Decimal::ToInt32(numLuzX->Value);
			int yLight = System::Decimal::ToInt32(numLuzY->Value);
			int zLight = System::Decimal::ToInt32(numLuzZ->Value);
			int Shine = System::Decimal::ToInt32(numShine->Value);
		
			int xObs = System::Decimal::ToInt32(numObsX->Value);
			int yObs = System::Decimal::ToInt32(numObsY->Value);
			int zObs = System::Decimal::ToInt32(numObsZ->Value);

			float percentil = System::Decimal::ToDouble(numPerc->Value);
			int increase_pixel = System::Decimal::ToInt32(numIncr->Value);
			bool enableSpecular = ckbSpecular->Checked;
			bool withBorder = ckbWithBorder->Checked;
			bool filled = ckbFilled->Checked;
			float ambiente = System::Decimal::ToDouble(numSilhueta->Value);
			int hatchingSize =System::Decimal::ToInt32(numSize->Value);

			strOriginal = (char*)(void*)Marshal::StringToHGlobalAnsi(this->originalFilename);

			imgOriginal = GetImage(strOriginal);

			txtLog->Text += formatLog("Iniciando Phong Gray ");
			IplImage *imgWhite       = processWhite(imageColor);
			IplImage *imgPhongGray = processPhong(imgWhite, xLight, yLight, zLight, Shine, xObs, yObs, zObs, regions.size, regions.regions, enableSpecular, ambiente, false, imgOriginal);
			txtLog->Text += formatLog("Finalizando Phong Gray ");

			pictureBox10->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			pictureBox10->Image = ImageToBitmap(imgPhongGray);
			currentImage = pictureBox10->Image;
			pictureBox10->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			tabPagPhongGray->Show();
			tabPagPhongGray->Enabled = true;
			tabPagPhongGray->Visible = true;
			tabControl2->SelectedTab = tabPagPhongGray;
			
			txtLog->Text += formatLog("Iniciando Phong Color ");
			IplImage *imgPhongColor  = processPhong(imageColor,xLight, yLight, zLight, Shine, xObs, yObs, zObs, regions.size, regions.regions, enableSpecular, ambiente, true, imgOriginal);

			pictureBox8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			pictureBox8->Image = ImageToBitmap(imgPhongColor);
			currentImage = pictureBox8->Image;
			pictureBox8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			tabPagPhong->Show();
			tabPagPhong->Enabled = true;
			tabPagPhong->Visible = true;
			tabControl2->SelectedTab = tabPagPhong;
			txtLog->Text += formatLog("Finalizando Phong Color ");

			txtLog->Text += formatLog("Iniciando Phong Render ");
			IplImage *imgPhongRender = processPhong(imageRenderColor, xLight, yLight, zLight, Shine, xObs, yObs, zObs, regions.size, regions.regions, enableSpecular, ambiente, false, imgOriginal);
			txtLog->Text += formatLog("Finalizando Phong Render ");
			pictureBox11->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			pictureBox11->Image = ImageToBitmap(imgPhongRender);
			currentImage = pictureBox11->Image;
			pictureBox11->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			tabPagPhongRender->Show();
			tabPagPhongRender->Enabled = true;
			tabPagPhongRender->Visible = true;
			tabControl2->SelectedTab = tabPagPhongRender;

			txtLog->Text += formatLog("Iniciando Threshold ");

			IplImage *imgThreshold = processThreshold(imgOriginal, imgPhongGray, imageRenderColor, regions.regions, regions.size, percentil, increase_pixel, withBorder, xLight, yLight, filled,false, hatchingSize);
			txtLog->Text += formatLog("Finalizand Threshold ");
			pictureBox9->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			pictureBox9->Image = ImageToBitmap(imgThreshold);
			currentImage = pictureBox9->Image;
			pictureBox9->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			tabPagThreshold->Show();
			tabPagThreshold->Enabled = true;
			tabPagThreshold->Visible = true;
			tabControl2->SelectedTab = tabPagThreshold;

			txtLog->Text += formatLog("Iniciando Threshold ");

			IplImage *imgThresholdRender = processThreshold(imgOriginal, imgPhongGray, imgPhongRender, regions.regions, regions.size, percentil, increase_pixel, withBorder, xLight, yLight, filled,true, hatchingSize);
			txtLog->Text += formatLog("Finalizando Threshold Render ");
			pictureBox12->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			pictureBox12->Image = ImageToBitmap(imgThresholdRender);
			currentImage = pictureBox12->Image;
			pictureBox12->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			tabPagThresholdRender->Show();
			tabPagThresholdRender->Enabled = true;
			tabPagThresholdRender->Visible = true;
			tabControl2->SelectedTab = tabPagThresholdRender;
		 }
private: System::Void tabPagPhongGray_Enter(System::Object^  sender, System::EventArgs^  e) {
 			currentImage = pictureBox10->Image;
		 }
private: System::Void pictureBox8_Click(System::Object^  sender, System::EventArgs^  e) {
 			
		 }
private: System::Void pictureBox11_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pictureBox9_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void tabPagPhong_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox8->Image;
		 }
private: System::Void tabPagPhongRender_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox11->Image;
		 }
private: System::Void tabPagThreshold_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox9->Image;
		 }
private: System::Void tabPagThresholdRender_Enter(System::Object^  sender, System::EventArgs^  e) {
			 currentImage = pictureBox12->Image;
		 }
private: System::Void tabPagDepth_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void tabPagNormal2_Enter(System::Object^  sender, System::EventArgs^  e) {
			 	currentImage = pictNormal2->Image;
		 }
private: System::Void groupBox3_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void toolStripButton1_Click_1(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pictureBox13_MouseCaptureChanged(System::Object^  sender, System::EventArgs^  e) {
		 }


private: System::Void pictureBox13_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			if((drawOperations & DRAW_OPER_TRACK) == DRAW_OPER_TRACK && clickMouse)
			{
				if(vecX.size() == 0)
				{
					g->DrawLine(System::Drawing::Pens::Black,e->X,e->Y,e->X,e->Y);
				}
				else
				{
					g->DrawLine(System::Drawing::Pens::Black,vecX.at(vecX.size()-1),vecY.at(vecY.size()-1),e->X,e->Y);
				}

				vecX.push_back(e->X);
				vecY.push_back(e->Y);
			}
			if((drawOperations & DRAW_OPER_MOVE)  == DRAW_OPER_MOVE && clickMouse && lastRegion != -1)
			{
				int diffPosX = e->X - lastPosX;
				int diffPosY = e->Y - lastPosY;
				lastPosX = e->X;
				lastPosY = e->Y;
				totalDiffX += diffPosX;
				totalDiffY += diffPosY;
				
				for(int i = 0; i < regions.regions[lastRegion].normal.contour_arr_x.size()-1;i++)
				{
					g->DrawLine(System::Drawing::Pens::Black, regions.regions[lastRegion].normal.contour_arr_x.at(i)+totalDiffX, regions.regions[lastRegion].normal.contour_arr_y.at(i)+totalDiffY, regions.regions[lastRegion].normal.contour_arr_x.at(i+1)+totalDiffX, regions.regions[lastRegion].normal.contour_arr_y.at(i+1)+totalDiffY);
				}
			}
		 }
private: System::Void pictureBox13_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		 }
private: System::Void pictureBox13_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(e->Button == System::Windows::Forms::MouseButtons::Right)
				 return;

			 clickMouse = true;

			 if((drawOperations & DRAW_OPER_TRACK) == DRAW_OPER_TRACK)
			 {
				 vecX.clear();
				 vecY.clear();
			 }

			 if((drawOperations & DRAW_OPER_SELECT) == DRAW_OPER_SELECT)
			 {
				 lastPosX = e->X;
				 lastPosY = e->Y;
				 totalDiffX = 0;
				 totalDiffY = 0;
				 lastRegion = -1;
				 if(!((mapindex[e->X].size() == 0) || (mapindex[e->X].size() != 0 && mapindex[e->X][e->Y].size() != 1)))
				 {
					lastRegion = mapindex[e->X][e->Y].at(0);
				 }
				 

			 }
			 g = pictureBox13->CreateGraphics();

		 }
void Bresenham(int x1,
    int y1,
    int x2,
    int y2, vector<int> &vX, vector<int> &vY)
{
    signed char ix;
    signed char iy;
 
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int delta_x = (x2 > x1?(ix = 1, x2 - x1):(ix = -1, x1 - x2)) << 1;
    int delta_y = (y2 > y1?(iy = 1, y2 - y1):(iy = -1, y1 - y2)) << 1;

	vX.push_back(x1);
	vY.push_back(y1);

	if (delta_x >= delta_y)
    {
        // error may go below zero
        int error = delta_y - (delta_x >> 1);
 
        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing
 
            x1 += ix;
            error += delta_y;
 
			vX.push_back(x1);
			vY.push_back(y1);

        }
    }
    else
    {
        // error may go below zero
        int error = delta_x - (delta_y >> 1);
 
        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing
 
            y1 += iy;
            error += delta_x;
 
			vX.push_back(x1);
			vY.push_back(y1);
        }
    }
}

private: System::Void pictureBox13_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 IplImage *imgNormal = NULL;
			 IplImage *imgBres = NULL;
			 
			 clickMouse = false;

			 if(e->Button == System::Windows::Forms::MouseButtons::Right)
				 return;
			 
			 int oldRegion = -1;
			 int region_id;

			 vector<int> nX;
			 vector<int> nY;

			 vector<int> subX;
			 vector<int> subY;

			 if((drawOperations & DRAW_OPER_TRACK) == DRAW_OPER_TRACK)
			 {
				imgBres = processWhite(imageColor);

				vecX.push_back(e->X);
				vecY.push_back(e->Y);

				if(vecX.size() != 0)
				{
					int j;
					nX.clear();
					nY.clear();

					for(j = 0; j < vecX.size()-1; j++)
					{
						Bresenham(vecX[j], vecY[j],vecX[j+1], vecY[j+1], nX, nY);
					}
					if((drawOperations & DRAW_OPER_NEW_REGION) == DRAW_OPER_NEW_REGION)
						Bresenham(vecX[j], vecY[j],vecX[0], vecY[0], nX, nY);
				}	
				 vecX.clear();
				 vecY.clear();
				 for(int j = 1; j < nX.size();j++)
				 {
					if(nX[j-1] == nX[j] && nY[j-1] == nY[j])
						continue;
					vecX.push_back(nX[j]);
					vecY.push_back(nY[j]);
				 }
				 nX.clear();
				 nY.clear();

				 for(int i = 0; i < vecX.size();i++)
				 {
					 SET_PIXELRGB(imgBres,vecX[i],vecY[i],0,0);
					 SET_PIXELRGB(imgBres,vecX[i],vecY[i],0,1);
					 SET_PIXELRGB(imgBres,vecX[i],vecY[i],0,2);
				 }
				 cvSaveImage("bresenham.png",imgBres);
			 }
			 if((drawOperations & DRAW_OPER_NEW_REGION) == DRAW_OPER_NEW_REGION)
			 {
				 bool found = false;
				 int x = 0, y = 0, p;

				 for(int i = 0; i < vecX.size(); i++)
				 {
					 x = vecX[i]+1;
					 y = vecY[i]+1;
					 GET_PIXEL4INT(imgBres,x,y, &p);
					 if(p == WHITE_COLOR && beContained(x, y, vecX, vecY, imgBres->height, imgBres->width))
					 {
						found = true;
						break;
					 }
				 }
				 int r = doFloodFillNewRegion(imgBres,x,y,regions.regions);
				 regions.size = r--;
				 for(int i = 0; i < vecX.size();i++)
				 {
					 regions.regions[r].contour_arr_x.push_back(vecX[i]);
					 regions.regions[r].contour_arr_y.push_back(vecY[i]);
					 mapindex[vecX[i]][vecY[i]].push_back(r);
				 }

				 for(int i = 0; i < regions.regions[r].fill_arr_x.size();i++)
				 {
					 SET_PIXEL4INT(imgBres,regions.regions[r].fill_arr_x[i],regions.regions[r].fill_arr_y[i],0);
				 }

				 vecX.clear();
				 vecX.clear();
				 pictureBox13->Image = ImageToBitmap(imgBres);

				 normalize_plus(regions.regions[r], regions.regions, regions.size);
				 imgNormal = processNormalizeInc(imageColor, &regions);
				 pictureBox13->Image = ImageToBitmap(imgNormal);
				 currentImage = pictureBox13->Image;
				 vecX.clear();
				 vecX.clear();
			 }
			 
			 if((drawOperations & DRAW_OPER_LINE) == DRAW_OPER_LINE)
			 {
				 for(int i = 0; i < vecX.size(); i++)
				 {
					 if((mapindex[vecX[i]].size() == 0) || (mapindex[vecX[i]].size() != 0 && mapindex[vecX[i]][vecY[i]].size() != 1))
					 {
						normalize_inc(regions.regions[region_id], subX, subY);
						subX.clear();
						subY.clear();
						oldRegion = -1;
						continue;
					 }

					 region_id = mapindex[vecX[i]][vecY[i]][0];

					 if(region_id != oldRegion)
					 {
						oldRegion = region_id;
						if(subX.size() == 0)
						{
							 subX.push_back(vecX[i]);
							 subY.push_back(vecY[i]);
							 continue;
						}
						normalize_inc(regions.regions[region_id], subX, subY);
						subX.clear();
						subY.clear();
					 }
					 subX.push_back(vecX[i]);
					 subY.push_back(vecY[i]);
				 }

				 if(subX.size() != 0)
				 {
					normalize_inc(regions.regions[region_id], subX, subY);
				 }
				 imgNormal = processNormalizeInc(imageColor, &regions);
				 pictureBox13->Image = ImageToBitmap(imgNormal);
				 currentImage = pictureBox13->Image;

				 subX.clear();
				 subY.clear();
			 }
			
			 if((drawOperations & DRAW_OPER_MOVE) == DRAW_OPER_MOVE)
			 {
				 for(int i = 0; i < regions.regions[lastRegion].normal.contour_arr_x.size();i++)
				 {
				    mapindex[regions.regions[lastRegion].normal.contour_arr_x[i]][regions.regions[lastRegion].normal.contour_arr_y[i]].clear();
					regions.regions[lastRegion].normal.contour_arr_x[i] += totalDiffX;
					regions.regions[lastRegion].normal.contour_arr_y[i] += totalDiffY;
					mapindex[regions.regions[lastRegion].normal.contour_arr_x[i]][regions.regions[lastRegion].normal.contour_arr_y[i]].push_back(lastRegion);
				 }

				 for(int i = 0; i < regions.regions[lastRegion].normal.fill_arr_x.size();i++)
				 {
					mapindex[regions.regions[lastRegion].normal.fill_arr_x[i]][regions.regions[lastRegion].normal.fill_arr_y[i]].clear();
					regions.regions[lastRegion].normal.fill_arr_x[i] += totalDiffX;
					regions.regions[lastRegion].normal.fill_arr_y[i] += totalDiffY;
					mapindex[regions.regions[lastRegion].normal.fill_arr_x[i]][regions.regions[lastRegion].normal.fill_arr_y[i]].push_back(lastRegion);
				 }

				 imgNormal = processNormalizeInc(imageColor, &regions);
				 pictureBox13->Image = ImageToBitmap(imgNormal);
				 currentImage = pictureBox13->Image;
				 lastRegion = -1;
			 }

		 }
private: System::Void newRegionsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 drawOperations = DRAW_NEW_REGION;
			 newRegionItem->Checked = true;
			 StatusBar->Text = "New Regions";
		 }
private: System::Void lineToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 drawOperations = DRAW_LINE;
			 lineItem->Checked = true;
			 StatusBar->Text = "Line In";
		 }
private: System::Void contextMenuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
			 newRegionItem->Checked = false;
			 lineItem->Checked = false;
			 moveRegionItem->Checked = false;
		 }
private: System::Void moveRegionItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 drawOperations = DRAW_MOVE_REGION;
			 moveRegionItem->Checked = true;
			 StatusBar->Text = "Move Region";
		 }
private: System::Void toolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e) {
			 IplImage *imgScalar = processNormalizeScalar(imageColor, &regions,0);
			 pictureBox13->Image = ImageToBitmap(imgScalar);
			 currentImage = pictureBox13->Image;
		 }
private: System::Void Scalaritem_Click(System::Object^  sender, System::EventArgs^  e) {
			 IplImage *imgScalar = processNormalizeScalar(imageColor, &regions,1);
			 pictureBox13->Image = ImageToBitmap(imgScalar);
			 currentImage = pictureBox13->Image;
		 }
private: System::Void toolStripMenuItem3_Click(System::Object^  sender, System::EventArgs^  e) {
			 IplImage *imgScalar = processNormalizeScalar(imageColor, &regions,10);
			 pictureBox13->Image = ImageToBitmap(imgScalar);
			 currentImage = pictureBox13->Image;
		 }
private: System::Void toolStripMenuItem4_Click(System::Object^  sender, System::EventArgs^  e) {
			 IplImage *imgScalar = processNormalizeScalar(imageColor, &regions,100);
			 pictureBox13->Image = ImageToBitmap(imgScalar);
			 currentImage = pictureBox13->Image;

		 }
};
}

