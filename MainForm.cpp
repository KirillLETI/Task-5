//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define NAME_SKEY 0
#define YEAR_SKEY 1
#define PRICE_SKEY 2

#include <cmath>

using namespace std;
//---------------------------------------------------------------------------
TLinkMainForm *LinkMainForm;


//---------------------------------------------------------------------------
__fastcall TLinkMainForm::TLinkMainForm(TComponent* Owner)
	: TForm(Owner)
{
	SortingCB->ItemIndex = 0;
}

void TLinkMainForm::Read_Data(char* filename)
{
	FILE *inp = fopen(filename,"r");

	if(!inp)
	{
		ShowMessage("Ошибка открытия файла!");
		return;
	}

	char 	type[LEN];
	char 	name[LEN];
	int 	year;
	double 	price;

	while(!feof(inp))
	{
		fscanf(inp, "%s", type);

		if ( !memcmp(type, "FUEL", 4) )
		{
			char 	ftype[LEN];
			float 	tvol;
			float 	frate;

			if( fscanf(inp, "%s %d %lf %s %f %f\n", name, &year, &price,
				ftype, &tvol, &frate) )
			{
				cars[cnt] = new Fuel_Engine(type, name, year, price,
					ftype, tvol, frate);
			}
			else
			{
				ShowMessage(cars[cnt]->GetTYPE());
				ShowMessage("Ошибка в чтении данных!");
			 }
		}

		else if ( !memcmp(type, "ELECT", 5) )
		{
			char 	eltype[LEN];
			int 	btcpt;
			int 	ctime;

			if (fscanf(inp, "%s %d %lf %s %d %d\n", name, &year, &price,
				eltype, &btcpt, &ctime) )
			{
				cars[cnt] = new Electric_Engine(type, name, year, price,
					eltype, btcpt, ctime);
			}
			else
			{
				ShowMessage(cars[cnt]->GetTYPE());
				ShowMessage("Ошибка в чтении данных!");
			}
		}
		cnt++;
	}

	fclose(inp);

	ShowMessage("Данные прочитаны успешно!");
	Show_Data();
}

void TLinkMainForm::Updata_Object()
{
	AnsiString index_str = DetailedForm->IndexL->Caption;
	AnsiString N = DetailedForm->NameEd->Text;
	AnsiString Y = DetailedForm->YearEd->Text;
	AnsiString P = DetailedForm->PriceEd->Text;

	AnsiString A1 = DetailedForm->AddEd1->Text;
	AnsiString A2 = DetailedForm->AddEd2->Text;
	AnsiString A3 = DetailedForm->AddEd3->Text;

	int index = atoi(index_str.c_str());

	AnsiString T;

	T = AnsiString(cars[index]->GetTYPE());

	if ( !memcmp(cars[index]->GetTYPE(), "FUEL", 4) )
		{
			Car* buf =  new Fuel_Engine(T, N, Y, P, A1, A2, A3);

			cars[index] = buf;

			delete [] buf;

		}

	else if ( !memcmp(cars[index]->GetTYPE(), "ELECT", 5) )
		{
			Car* buf = new Electric_Engine(T, N, Y, P, A1, A2, A3);

			 cars[index] = buf;

			delete [] buf;
		}

	Show_Data();

	ShowMessage("Данные изменены!");

}

void TLinkMainForm::Add_Object()
{
	AnsiString N = DetailedForm->NameEd->Text;
	AnsiString Y = DetailedForm->YearEd->Text;
	AnsiString P = DetailedForm->PriceEd->Text;

	AnsiString A1 = DetailedForm->AddEd1->Text;
	AnsiString A2 = DetailedForm->AddEd2->Text;
	AnsiString A3 = DetailedForm->AddEd3->Text;

	AnsiString T = (!DetailedForm->TypeCb->ItemIndex) ? "FUEL" : "ELECT";


	if ( !memcmp(T.c_str(), "FUEL", 4) )
		{
			cars[cnt] = new Fuel_Engine(T, N, Y, P, A1, A2, A3);
		}

	else if ( !memcmp(T.c_str(), "ELECT", 5) )
		{
			cars[cnt] = new Fuel_Engine(T, N, Y, P, A1, A2, A3);
		}
	cnt++;
	Show_Data();
	ShowMessage("Данные добавлены!");
}

void TLinkMainForm::Show_Data(Car* array[], int n)
{
	AutoList->Items->BeginUpdate();
	AutoList->Items->Clear();

	for (int i = 0; i < n; i++)
	{

		TListItem* item = AutoList->Items->Add();

		item->Caption = array[i]->GetName();

		item->Data = (void*)i;

		int dataValue = (int)item->Data; // запоминаем индекс

		item->SubItems->Add(array[i]->GetYear());
		item->SubItems->Add(array[i]->GetPrice());

		char buf[100];

		if ( !memcmp(array[i]->GetTYPE(), "FUEL", 4) )
		{
			sprintf(buf, "тип топлива: %s, объём бака: %g л, расход: %g л",
				array[i]->GetFuel_Type(), array[i]->GetTank_Vol(),
					array[i]->GetFuel_Rate() );

			item->SubItems->Add(buf);
		}

		else if ( !memcmp(array[i]->GetTYPE(), "ELECT", 5) )
		{
			sprintf(buf, "тип двигателя: %s, батарея: %d кВТ/ч, зарядка: %d часов",
				array[i]->GetEl_type(), array[i]->GetBat_cpt(),
					array[i]->GetCh_time());

			item->SubItems->Add(buf);
		}
	}

	AutoList->Items->EndUpdate();
}

void TLinkMainForm::Show_EditMenu(int index)
{
	DetailedForm->Head1L->Visible = true;
	DetailedForm->Head2L->Visible = false;

	DetailedForm->TypeCb->Visible = false;
	DetailedForm->AddEd1->Enabled = true;
	DetailedForm->AddEd2->Enabled = true;
	DetailedForm->AddEd3->Enabled = true;

	if ( !memcmp(cars[index]->GetTYPE(), "FUEL", 4) )
	{
		DetailedForm->AddL1->Caption = "Тип топлива" ;
		DetailedForm->AddL2->Caption = "Объём бака, л" ;
		DetailedForm->AddL3->Caption = "Расход, л";
	}

	else if ( !memcmp(cars[index]->GetTYPE(), "ELECT", 5) )
	{
		DetailedForm->AddL1->Caption = "Тип двигателя" ;
		DetailedForm->AddL2->Caption = "Батарея, кВт/ч" ;
		DetailedForm->AddL3->Caption = "Зарядка, ч";
	}

	char* 	properties = new char[100];

	char 	name[LEN];
	char 	year[LEN];
	char	price[LEN];
	char	add1[LEN];
	char 	add2[LEN];
	char 	add3[LEN];

	cars[index]->GetValues(properties);

	sscanf(properties, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", name,
		year, price, add1, add2, add3);

	DetailedForm->NameEd->Text = name;
	DetailedForm->YearEd->Text = year;
	DetailedForm->PriceEd->Text = price;
	DetailedForm->AddEd1->Text = add1;
	DetailedForm->AddEd2->Text = add2;
	DetailedForm->AddEd3->Text = add3;

	delete[] properties;
}

void TLinkMainForm::Save_Data(char* filename)
{
	FILE *inp = fopen(filename,"w");

	if(!inp)
	{
		ShowMessage("Ошибка открытия файла!");
		return;
	}

	for (int i = 0; i < cnt; i++)
	{
		fprintf(inp, "%s %s %d %lf ", cars[i]->GetTYPE(), cars[i]->GetName(),
			cars[i]->GetYear(), cars[i]->GetPrice());

		if (!memcmp(cars[i]->GetTYPE(), "FUEL", 4))

			fprintf(inp, "%s %f %f\n", cars[i]->GetFuel_Type(),
				cars[i]->GetTank_Vol(), cars[i]->GetFuel_Rate());

		else if (!memcmp(cars[i]->GetTYPE(), "ELECT", 5))

			fprintf(inp, "%s %d %d\n", cars[i]->GetEl_type(),
				cars[i]->GetBat_cpt(), cars[i]->GetCh_time());
	}

	fclose(inp);

	ShowMessage("Данные успешно записаны!");
}

void TLinkMainForm::QuickSorting(int sort_key, Car* array[])
{
	sort(sort_key, 0, cnt-1, array);
}

void TLinkMainForm::sort(int key, int L, int R, Car* array[])
{
	if (L >= R) return;


	int x = min( {L, R, (L + R) / 2});

	int i = L;
	int j = R;

	while(i <= j)
	{
		while(sorting_key(key, i, x, array)) i++;
		while(sorting_key(key, x, j, array)) j--;

		if (i <= j)
		{
			Car* buf = 	array[i];
			array[i]  = array[j];
			array[j]  =	buf;

			i++; j--;
		}
	}

	if(L < j) sort(key, L, j, array);
	if(i < R) sort(key, i, R, array);
}

void TLinkMainForm::BubleSort(int sort_key, Car* array[])
{
	for(int i = 1; i < cnt; i++)
		for (int j = cnt - 1; j >= i; j--)
			if (sorting_key(sort_key, j, j-1, array))
			{
				Car* buf = array[j-1];
				array[j-1] = array[j];
				array[j] = buf;
			}
}

void TLinkMainForm::Clear_Data()
{
	for (int i = 0; i < cnt; i++)
	{
		delete cars[i];
		cars[i] = NULL;
	}

	cnt = 0;
	Show_Data();
	ShowMessage("Данные очищены!");
}

void TLinkMainForm::GlobalSearch(Car* array[], int x, int* res, int &k)
{

	int L = 0, R = cnt - 1;

	while(L <= R)
	{
		int a = (L+R)/2;

		if (array[a]->GetYear() < x)
			L = a+1;
		else if(array[a]->GetYear() > x)
			 R = a-1;
		else
		{
			res[k] = a;
			k++;
			break;
		}
	}

	int l = res[0] - 1;
	while (l >= 0 && array[l]->GetYear() == x) {
		res[k] = l;
		k++;
		l--;
	}

	int r = res[0] + 1;
	while (r < cnt && array[r]->GetYear() == x) {
		res[k] = r;
		k++;
		r++;
	}
}

void TLinkMainForm::Show_CreateMenu()
{
	DetailedForm->Head1L->Visible = false;
	DetailedForm->Head2L->Visible = true;

	DetailedForm->TypeCb->Visible = true;

	DetailedForm->NameEd->Text 	= "";
	DetailedForm->PriceEd->Text = "";
	DetailedForm->YearEd->Text 	= "";

	DetailedForm->AddL1->Caption = "н/о";
	DetailedForm->AddEd1->Text = "";
	DetailedForm->AddEd1->Enabled = false;

	DetailedForm->AddL2->Caption = "н/о";
	DetailedForm->AddEd2->Text = "";
	DetailedForm->AddEd2->Enabled = false;

	DetailedForm->AddL3->Caption = "н/о";
	DetailedForm->AddEd3->Text = "";
	DetailedForm->AddEd3->Enabled = false;
}

void TLinkMainForm::Pop_Object(int index)
{
		delete cars[index];

		for (int i = index; i < cnt - 1; i++)
		{
			cars[i] = cars[i + 1];
		}

		cnt--;
		Show_Data();
}

bool TLinkMainForm::sorting_key(int key, int x, int i, Car* array[]) {

	switch (key)
	{
	case NAME_SKEY:
		return strcmp(array[x]->GetName(), array[i]->GetName()) < 0;
		break;

	case YEAR_SKEY:
		return array[x]->GetYear() < array[i]->GetYear();
		break;

	case PRICE_SKEY:
		return array[x]->GetPrice() < array[i]->GetPrice();
		break;

    default:
		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::GoBackBtClick(TObject *Sender)
{
	GoBackBt->Visible = false;
	SearchEdit->Text = "";
	Show_Data();
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::Save(TObject *Sender)
{
	char filename[MAX_PATH];

	if (FilesDialog->Execute())
	{
		StrPCopy(filename, FilesDialog->FileName.c_str());
		Save_Data(filename);
	}
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::ReadData(TObject *Sender)
{
	char filename[MAX_PATH];

	if (FilesDialog->Execute())
	{
		StrPCopy(filename, FilesDialog->FileName.c_str());
		Read_Data(filename);
	}
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::ExitApp(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::ClearAuto(TObject *Sender)
{
	Clear_Data();
}
//---------------------------------------------------------------------------int X = 520;
void __fastcall TLinkMainForm::EditProperties(TObject *Sender)
{

	if(AutoList->Selected == nullptr || cnt == 0) { return; }

	DetailedForm->Visible = true;

	int index = (int)((TListItem*)AutoList->Selected)->Data; //индекс

	Show_EditMenu(index);

	DetailedForm->IndexL->Visible = false;

	DetailedForm->IndexL->Caption = index;
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::SortActivation(TObject *Sender)
{
	int sort_key = SortingCB->ItemIndex;

	QuickSorting(sort_key, cars);
	Show_Data();
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::SearchAct(TObject *Sender, System::WideChar &Key)
{
	if (Key != VK_RETURN) { return; }

	char buf[LEN];
	AnsiString temp;

	temp = SearchEdit->Text;
	int x = atoi(temp.c_str());

	Car* BUF[LEN], *RES[LEN];

	for(int i = 0; i < cnt; i++) {BUF[i] = cars[i];}

	int res[LEN], k = 0;

	QuickSorting(YEAR_SKEY, BUF);

	GlobalSearch(BUF, x, res, k);

	for (int i = 0; i < k; i++) { RES[i] = BUF[res[i]];}

   Show_Data(RES, k);

   GoBackBt->Visible = true;

   Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::InsrertClick(TObject *Sender)
{
	if(cnt == LEN)
	{
		ShowMessage("Массив переполнен!");
		return;
	}

	DetailedForm->Visible = true;

	Show_CreateMenu();
}
//---------------------------------------------------------------------------
void __fastcall TLinkMainForm::PopClick(TObject *Sender)
{
	if(AutoList->Selected == nullptr || cnt == 0) { return; }

	int index = (int)(AutoList->Selected)->Data; //индекс

	Pop_Object(index);
}
//---------------------------------------------------------------------------

//constructors
Car::Car(char* Type, char* Name, int Year, double Price)
{
	strcpy(TYPE, Type);
	strcpy(name, Name);
	year = Year;
	price = Price;
}

Car::Car(AnsiString Type, AnsiString Name, AnsiString Year,
	AnsiString Price)
{
	strcpy(TYPE, Type.c_str());
	strcpy(name, Name.c_str());
	year = atoi(Year.c_str());
	price = atof(Price.c_str());
}

Car::Car(const Car& C)
{
	strcpy(TYPE, C.TYPE);
	strcpy(name, C.name);
	year = C.year;
	price = C.price;
}

Fuel_Engine::Fuel_Engine(char* Type, char* name, int year, double price,
	char* ftype, float tvol, float frate) : Car(Type, name, year, price)
{
	 strcpy(fuel_type, ftype);
	 tank_volume = tvol;
	 fuel_rate = frate;
}

Fuel_Engine::Fuel_Engine(AnsiString Type, AnsiString Name, AnsiString Year,
		AnsiString Price, AnsiString ftype, AnsiString tvol,
			AnsiString frate) : Car(Type, Name, Year, Price)
{
	strcpy(fuel_type, ftype.c_str());
	tank_volume = atof(tvol.c_str());
	fuel_rate = atof(frate.c_str());
}

Fuel_Engine::Fuel_Engine(const Fuel_Engine& F) : Car(F)
{
	strcpy(fuel_type, F.fuel_type);
	tank_volume = F.tank_volume;
	fuel_rate = F.fuel_rate;
}

Electric_Engine::Electric_Engine(char* Type, char* name, int year,
	double price, char* eltype, int btcpt, int ctime) :
		Car(Type, name, year, price)
{
	strcpy(elengine_type, eltype);
	battery_capacity = btcpt;
	charging_time = ctime;
}

Electric_Engine::Electric_Engine(AnsiString Type, AnsiString Name, AnsiString Year,
		AnsiString Price, AnsiString eltype, AnsiString btcpt,
			AnsiString ctime) : Car(Type, Name, Year, Price)
{
	strcpy(elengine_type, eltype.c_str());
	battery_capacity = atoi(btcpt.c_str());
	charging_time = atoi(ctime.c_str());
}

Electric_Engine::Electric_Engine(const Electric_Engine& E) : Car(E)
{
	strcpy(elengine_type, E.elengine_type);
	battery_capacity = E.battery_capacity;
	charging_time = E.charging_time;
}
//class methods

void Fuel_Engine::GetValues(char* buf)
{
	sprintf(buf, "%s,%d,%lg,%s,%g,%g", GetName(), GetYear(), GetPrice(),
		GetFuel_Type(), GetTank_Vol(), GetFuel_Rate());
}

void Electric_Engine::GetValues(char* buf)
{
	sprintf(buf, "%s,%d,%lg,%s,%d,%d", GetName(), GetYear(), GetPrice(),
		GetEl_type(), GetBat_cpt(), GetCh_time());
}

//---------------------------------------------------------------------------


void __fastcall TLinkMainForm::pbPaint(TObject *Sender)
{
	if (!cnt) return;
	float w = pb->ClientWidth;
	float h = pb->ClientHeight;
	float x0 = w / 2;
	float y0 = h / 2;
	float r = min(x0, y0) * 0.8;

	double total = 0;
	for(int i = 0; i < cnt; i++)
	{
		total += cars[i]->GetYear();
	}

	Car* temp[LEN];
	for (int i = 0; i < cnt; i++) { temp[i] = cars[i];}

	QuickSorting(YEAR_SKEY, temp);

	int data[cnt][2];
	int ind = 0, i = 0;

	while (i < cnt)
	{
		int k = 1;
		int year = temp[i]->GetYear();

		int j = i + 1;
		while (j < cnt && temp[j]->GetYear() == year)
		{
			k++;
			j++;
		}

		data[ind][0] = year;
		data[ind][1] = k;
		ind++;
		i = j;
	}

	double angl0 = 0;
	i = 0;

	double x1 = x0 - r;
	double y1 = y0 - r;
	double x2 = x0 + r;
	double y2 = y0 + r;

	double x3, x3_prev =  0;
	double y3, y3_prev =  0;

	double x4 = x2;
	double y4 = 0;
	int pad = 0;
	while (angl0 < 360 && i < ind)
	{
		double pi = (data[i][0]*data[i][1] / total) * 100;
		double angli = pi * 360/100;


		x3 = x0 + r*cos( (angli + angl0) * M_PI / 180.0);
		y3 = y0 + r*sin( (angli + angl0) * M_PI / 180.0);

		x4 = (!i)? x2 : x3_prev;
		y4 = (!i)? 0  : y3_prev;

		int red = rand() % 256;
		int green = rand() % 256;
		int blue = rand() % 256;

		pb->Canvas->Pen->Color = (TColor)RGB(red, green, blue);
		TColor color = (TColor)RGB(red, green, blue);
		pb->Canvas->Brush->Color = color;

		pb->Canvas->FillRect(Rect(100, 120+pad, 110 , 130 + pad));
		pb->Canvas->Pie(x1, y1, x2, y2, x3, y3, x4, y4);

		pb->Canvas->Brush->Color = clWhite;
		int labelX = 120;
		int labelY = 120+pad;
		pb->Canvas->Font->Size = 10;
		pb->Canvas->Font->Name = "Arial";
		char labelT[LEN];
		sprintf(labelT, "%d, %.0lf%%", data[i][0], pi);
		pb->Canvas->TextOut(labelX, labelY, labelT);

		x3_prev = x3;
		y3_prev = y3;

		angl0 += angli;
		pad += 40;
		i++;
	}

	pb->Canvas->Font->Size = 14; // Размер шрифта
	pb->Canvas->Font->Name = "Arial"; // Название шрифта
	pb->Canvas->Brush->Color = clWhite; // Цвет фона (если нужно)
	pb->Canvas->Pen->Color = clBlack; // Цвет текста

	pb->Canvas->TextOut(x0 - 200, y0-270, "Диаграмма годов выпуска автомобилей, %");

}
//---------------------------------------------------------------------------

void __fastcall TLinkMainForm::Button2Click(TObject *Sender)
{
	pb->Canvas->Brush->Color = clWhite;
	pb->Canvas->FillRect(pb->ClientRect);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


