#include<windows.h>
#include<vector>
#include<map>
#include<iostream>
#include<time.h>
#include<cstdio>
#include<math.h>
#include<vector>
#include "DxLib.h"
#include<queue>
#include<stack>
#include<set>
#include<string>
#include <regex>
#include"mecab.h"
#include"filemaker.h"

/////
#include "MTable.h"
#include "Analyze.h"
#include "RuleBase.h"
#include "Character.h"
#include "Main.h"
/////

#ifdef _DEBUG
	#pragma comment(lib,"libmecab.lib")
#else
	#pragma comment(lib,"libmecab.lib")
#endif

//#import <msxml6.dll>
const int M_X=40,M_Y=40;
const int MAX_X=60,MAX_Y=60;
const int MAP_X=640,MAP_Y=480;
const float walkspeed=0.12;
const float mwalkspeed=0.05;
using namespace std;
#define rep(i,n) for(int i=0;i<n;i++)
const int INF =100100100;

int mode=0;//0�͕��s 1�͉�b 2�̓��[�r�[ 3�͐퓬
int mobnum=0;//��b���Ă��郂�u�̔ԍ�
int linenum=0;//�Z���t�ԍ�
int hako=-1;
int flag[10]={1,0,0,0,0,0,0,0,0,0};//�t���O
int SWINhandle;

int syudo[100]={0,0,1,0,1,1,1,1,1,1,  0,0,0,1,1,1,1,1,1,1,	1,1,1,1,1,1,1,1,1,1,  1,0,0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	#pragma comment(lib,"libmecab.lib")
#else
	#pragma comment(lib,"libmecab.lib")
#endif


int cnt=0;


/*int puc=0;
word pickup[100];
int counter=0;
int ctomb=0;
char mystr[100][40];
int mcounter=0;
char bunstr[100][40];
int bcounter=0;
*/

int pointer=0;
vector<pair<string,int>> strlog;


/*int typecode(string s){
	if(s.find("����")!=string::npos){return 0;}
	else if(s.find("�`�e��")!=string::npos){return 1;}
	else if(s.find("������")!=string::npos){return 10;}
	else if(s.find("�`�e����")!=string::npos){return 2;}
	else if(s.find("����")!=string::npos){return 3;}
	else if(s.find("����")!=string::npos){return 4;}
	else if(s.find("����")!=string::npos){return 11;}
	return -1;
}
*/

#define MAX_INPUT_SIZE 1000
#define CURSOR_X 10
#define CURSOR_Y 600
#define W_space 10
#define WIN_SIDE_S 20
#define LINE_S (WIN_SIDE_S+50)//80�ȏ�
#define LINE_K 20
#define LINE_L 50//TAIL_SIZE�ȏ�
#define LINE_R 430 //480-TAIL_SIZE�ȉ�
#define TAIL_SIZE 40
#define MAX_MOJI 13

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int startmode=0;
    // �c�w���C�u�����̏�����
	ChangeWindowMode(TRUE);
	SetGraphMode(480,640,32);
    if( DxLib_Init() < 0 )
    {
        // �G���[�����������璼���ɏI��
        return -1 ;
    }
	
	FILE *logfile=fopen("logfile.txt","a");
	char kuchoname[100],yasanname[100];
	sprintf(kuchoname,"�I�E��");sprintf(yasanname,"���ꂩ");
	fprintf(logfile,"\n\n************************************\n");
	int whiteP = GetColor(255,255,255 ) ;
	int yellowP = GetColor(200,200,0 ) ;
	int blackP = GetColor(0,0,0 ) ;
	int blueP = GetColor(0,0,255 ) ;
	DrawString(200, 310 ,"�N�����c",whiteP);
	///////////////
	setlocale(LC_CTYPE, "");

	srand((unsigned) time(NULL));
    //mtRand=std::mt19937(devRand());
	//Character * sensei = new Character(std::string("aozora\\kokoro\\kokoro.markov"), std::string("F:\\��2��������\\bungo\\bungo\\aozora\\kokoro\\RuleBasePatterns.rx"),std::string("aozora\\kokoro\\\\kokoroserif.txt"));
    //std::cout<<mt.startSentence("���l")<<std::endl;
	/////////////
	char md[200];
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
    {
		ClearDrawScreen() ;
		DrawString(90, 310 ,"***Enter to start***",whiteP);
		DrawString(0, 500 ,"1����͂��ăG���^�[��mode:1",yellowP);
		DrawString(0, 520 ,"2����͂��ăG���^�[��mode:2",yellowP);
		//if>20)DrawString(0, 80+20*20 ,"etc...(�ڂ����̓t�@�C���������Ă�������)",yellowP);
		if(KeyInputString( CURSOR_X, CURSOR_Y,100/*������*2*/ , md ,FALSE/*esc�L�����Z��*/ )==1){
			break;
		}
		ScreenFlip();
	}
	sscanf(md,"%d",&startmode);
	if(startmode<0||startmode>2){startmode=0;}


	///////////////
	char filename[200];
	char mname[100];
	vector<string> fileview;
	int index=1001001;
	Character * sensei;
	if(startmode!=2){
		fileview=FIND_FILES();
		while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
		{
			ClearDrawScreen() ;
			DrawString(0, 0 ,"�����̔ԍ������Ă�������(.txt�͕s�v�ł�)",whiteP);
			DrawString(0, 40 ,"�������������t�@�C�����ꗗ����������",yellowP);
			rep(i,fileview.size()){
				if(i>=fileview.size())break;
				char v[300];
				sprintf(v,"%d:%s",i,fileview[i].c_str());
				DrawString(0, 60+i*20 ,v,yellowP);			
			}
			//if>20)DrawString(0, 80+20*20 ,"etc...(�ڂ����̓t�@�C���������Ă�������)",yellowP);
			if(KeyInputString( CURSOR_X, CURSOR_Y,100/*������*2*/ , mname ,FALSE/*esc�L�����Z��*/ )==1){
				break;
			}
			ScreenFlip();
		}
		DrawString(180, 310 ,"�ǂݍ��ݒ��c",whiteP);
		ScreenFlip();
		int index=1001001;sscanf(mname,"%d",&index);
		if(index<fileview.size())sprintf(mname,"%s",fileview[index].c_str());
		fprintf(logfile,"���F%s\t",mname);
		sprintf(filename,"text\\%s.txt",mname);
		RAY_BUNGO_DICTIONALY(filename);
		while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
		{
			ClearDrawScreen() ;
			DrawString(0, 0 ,"�����̔ԍ������Ă�������(.txt�͕s�v�ł�)",whiteP);
			DrawString(0, 40 ,"�������������t�@�C�����ꗗ����������",yellowP);
			rep(i,fileview.size()){
				if(i>=fileview.size())break;
				char v[300];
				sprintf(v,"%d:%s",i,fileview[i].c_str());
				DrawString(0, 60+i*20 ,v,yellowP);			
			}
			//if>20)DrawString(0, 80+20*20 ,"etc...(�ڂ����̓t�@�C���������Ă�������)",yellowP);
			if(KeyInputString( CURSOR_X, CURSOR_Y,100/*������*2*/ , mname ,FALSE/*esc�L�����Z��*/ )==1){
				break;
			}
			ScreenFlip();
		}
		DrawString(180, 310 ,"�ǂݍ��ݒ��c",whiteP);
		ScreenFlip();
		index=1001001;sscanf(mname,"%d",&index);
		if(index<fileview.size())sprintf(mname,"%s",fileview[index].c_str());
		sprintf(kuchoname,"%s",mname);
		fprintf(logfile,"�����F%s\t",mname);
		sprintf(filename,"text\\%s.txt",mname);
		RAY_BUNGO_LEARN(filename);
		////////////////////////////////////////////
		fileview.clear();
	}
	if(startmode!=1){
		fileview=FIND_YFILES();
		while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
		{
			ClearDrawScreen() ;
			DrawString(0, 0 ,"��l�ڂ̃t�@�C���ԍ������Ă�������(.txt�͕s�v�ł�)",whiteP);
			DrawString(0, 40 ,"�������������t�@�C�����ꗗ����������",yellowP);
			rep(i,fileview.size()){
				if(i>=fileview.size())break;
				char v[300];
				sprintf(v,"%d:%s",i,fileview[i].c_str());
				DrawString(0, 60+i*20 ,v,yellowP);			
			}
			//if>20)DrawString(0, 80+20*20 ,"etc...(�ڂ����̓t�@�C���������Ă�������)",yellowP);
			if(KeyInputString( CURSOR_X, CURSOR_Y,100/*������*2*/ , mname ,FALSE/*esc�L�����Z��*/ )==1){
				break;
			}
			ScreenFlip();
		}
		DrawString(180, 310 ,"�ǂݍ��ݒ��c",whiteP);
		ScreenFlip();
		index=1001001;sscanf(mname,"%d",&index);
		if(index<fileview.size())sprintf(mname,"%s",fileview[index].c_str());
		sprintf(yasanname,"%s",mname);
		char markovf[200];char rulebasef[200];char _seriff[200];
		fprintf(logfile,"��l�ځF%s\n",mname);
		sprintf(markovf,"data\\%s.markov",mname);
		sprintf(rulebasef,"RuleBasePatterns.rx");
		sprintf(_seriff,"data\\%s.serif",mname);
		sensei = new Character(std::string(markovf), std::string(rulebasef),std::string(_seriff));
		//////////////////////////////////////////
	}

	char input[MAX_INPUT_SIZE];	
	int haikei= LoadGraph( "picture\\PC2016bg2.jpg" ) ;
	int window1=LoadGraph("picture\\PC2016lic2.png");
	int window2=LoadGraph("picture\\PC2016lic.png");
	int tail1=LoadGraph("picture\\PC2016litail2.png");
	int tail2=LoadGraph("picture\\PC2016litail.png");
	int litwin1=LoadGraph("picture\\PC2016lit.png");
	int litwin2=LoadGraph("picture\\PC2016lib.png");
	int litwin3=LoadGraph("picture\\PC2016lit2.png");
	int litwin4=LoadGraph("picture\\PC2016lib2.png");
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
    {
		ClearDrawScreen() ;
		DrawGraph(0,0,haikei,FALSE);
		int L=1;//���ۂ̍s��
		rep(i,strlog.size()){
			pair<string,int> h=strlog[strlog.size()-i-1];
			int hsize=h.first.size();
			vector<string> Hyoji;
			int G=1;//�s��
			while(h.first.size()>MAX_MOJI*2){
				string fm=h.first.substr(0,MAX_MOJI*2);
				h.first.replace(0,MAX_MOJI*2,"");
				Hyoji.push_back(fm);
				G++;
				hsize=MAX_MOJI*2;
			}
			hsize=26;
			Hyoji.push_back(h.first);
			hsize*=10;
			int LUhasi= CURSOR_Y-LINE_S*(i+1)-LINE_K*(L+G)-W_space;
			
			if(h.second==0){//�����@�E�@�D�F
				DrawExtendGraph(LINE_R-hsize, LUhasi,LINE_R, LUhasi+LINE_K*G+W_space,window1,TRUE);
				DrawGraph(LINE_R, LUhasi ,tail1,TRUE);
				DrawGraph(LINE_R-hsize, LUhasi-WIN_SIDE_S ,litwin3,TRUE);
				DrawGraph(LINE_R-hsize,LUhasi+LINE_K*G+W_space ,litwin4,TRUE);
				SetFontSize(10);
				DrawString(LINE_R-hsize,LUhasi-WIN_SIDE_S ,"���Ȃ�",blackP);
				SetFontSize(20);
				rep(j,Hyoji.size()){
					DrawString(LINE_R-hsize, LUhasi+LINE_K*j+W_space ,Hyoji[j].c_str(),blackP);
				}
				
			}
			if(h.second==1){
				DrawExtendGraph(LINE_L, LUhasi, LINE_L+hsize, LUhasi+LINE_K*G+W_space,window2,TRUE);
				DrawGraph(LINE_L-TAIL_SIZE, LUhasi ,tail2,TRUE);
				DrawGraph(LINE_L, LUhasi-WIN_SIDE_S ,litwin1,TRUE);
				DrawGraph(LINE_L, LUhasi+LINE_K*G+W_space ,litwin2,TRUE);
				SetFontSize(10);
				DrawString(LINE_L, LUhasi-WIN_SIDE_S , yasanname,blackP);
				SetFontSize(20);
				rep(j,Hyoji.size()){
					DrawString(LINE_L, LUhasi+LINE_K*j+W_space ,Hyoji[j].c_str(),blackP);
				}
				
			}
			if(h.second==2){
				DrawExtendGraph(LINE_L, LUhasi, LINE_L+hsize, LUhasi+LINE_K*G+W_space,window2,TRUE);
				DrawGraph(LINE_L-TAIL_SIZE, LUhasi ,tail2,TRUE);
				DrawGraph(LINE_L, LUhasi-WIN_SIDE_S ,litwin1,TRUE);
				DrawGraph(LINE_L, LUhasi+LINE_K*G+W_space ,litwin2,TRUE);
				SetFontSize(10);
				DrawString(LINE_L, LUhasi-WIN_SIDE_S ,kuchoname,blackP);
				SetFontSize(20);
				rep(j,Hyoji.size()){
					DrawString(LINE_L, LUhasi+LINE_K*j+W_space ,Hyoji[j].c_str(),blueP);
				}
				
			}
			L+=G;

		}
		if(KeyInputString( CURSOR_X, CURSOR_Y,MAX_INPUT_SIZE/*������*2*/ , input ,FALSE/*esc�L�����Z��*/ )==1){
			
			
				fprintf(logfile,"���Ȃ�:\t%s\n",input);
				//DrawString(0,40,bunkai(mystr[mcounter-1]),whiteP);
				const MeCab::Node* node=nodecreate(input);
				pair<string,int> pinp;
				pinp.first=input;
				if(pinp.first!=""){
					pinp.second=0;
						strlog.push_back(pinp);
					if(startmode==0){
						
						//pinp.first=RAY_BUNGO_REPLY(input,false);pinp.second=2;
						pinp.first=sensei->Respond(std::string(input));
						pinp.second=1;
						if(pinp.first.size()==0){
							pinp.first=RAY_BUNGO_REPLY(input,true);
							pinp.second=2;
						}
						if(pinp.first.empty()||pinp.first=="�B"){
							pinp.first=sensei->ForceRespond(input);
							pinp.second=1;
						}
					
						////////////
						//pinp.first=sensei->Respond(std::string(pinp.first));
						//strlog.push_back(pinp);
						///////////
					}else if(startmode==1){
						int br=0;
						pinp.first=RAY_BUNGO_REPLY(input,true);
						pinp.second=2;
						while(pinp.first.empty()||pinp.first=="�B"){
							pinp.first=RAY_BUNGO_REPLY(input,false);
							pinp.second=2;
							br++;
							if(br>10)break;
						}br=0;
						while(pinp.first.empty()||pinp.first=="�B"){
							pinp.first=RAY_BUNGO_REPLY("������",false);
							pinp.second=2;
							if(br>15)break;
						}
					}else if(startmode==2){
						pinp.first=sensei->RespondF(input);
						pinp.second=1;
						if(pinp.first.empty()||pinp.first=="�B"){
							pinp.first=sensei->ForceRespond(input);
							pinp.second=1;
						}
					}	
					fprintf(logfile,"�V�X�e��%d:\t%s\n",pinp.second,pinp.first.c_str());
						strlog.push_back(pinp);
				}
					int p=0;
				/*for (; node; node = node->next) {
					//DrawString(0,40+20*p++,node->surface,whiteP);
					//DrawString(0,40+20*p++,node->feature,whiteP);
					string s=  string(node->surface);
					pickup[puc].surface=s.substr(0,node->length);
					s=string(node->feature);
					pickup[puc].type=node->posid+syudo[node->posid]*100;
					puc++;
					//DrawString(0,40+20*p++,"������������������������\n",whiteP);
					//std::cout << "==========" << std::endl;
				}*/
				
		}//���͂��������������̏����I���
		ScreenFlip();
	}

    DxLib_End() ;
    return 0 ;
}


