
//#include "matplotlibcpp.h"

using namespace std;
#define WEIGHT_NUMBER 39
#define rep(i,n) for(int i=0; i<n; i++)
#define randomsu -100+(double)(rand()*(200+ 1.0) / (1.0 + RAND_MAX) )
#define K_DEF (double)(1/kn*2)

string onlinelearnf;



string henkei(string s,int nandan,int bef, int aft);
class word{
public:
	int type;
	string surface;
	int katsuyou_type;
	int meishi_type;
	int ifnaiyo;
	int nandan;

	bool operator == (word another){
			return (bool)(this->surface==another.surface);
	}
	bool operator < (word another){
			return (bool)(this->surface<another.surface);
	}
	bool operator > (word another){
			return (bool)(this->surface>another.surface);
	}
};
struct imibl{
	string surface;
	int ftype;
	int fkatsuyou_type;
	int ltype;
	int lkatsuyou_type;
};

double naiseki(vector<double> a,vector<double> b){
	double r=0;
	rep(i,min(a.size(),b.size() ) ){
		r+= a[i]*b[i];
	}
	return r;
}
word wo0;//���̌��t
word wo1;//���̌��t
word wo2;//�O�̌��t
const char* bunkai(char *input){
	MeCab::Tagger *tagger = MeCab::createTagger("-d MeCab\\dic\\ipadic");
	return tagger->parse(input);
}

const MeCab::Node* nodecreate(char *input){
	MeCab::Tagger *tagger = MeCab::createTagger("-d MeCab\\dic\\ipadic");
	const MeCab::Node* node = tagger->parseToNode(input);
	if(tagger)delete tagger;
	return node;
}



class wordsort{
public:
	bool operator()(const word a,word another) const {
			return a.type<another.type;
	}
};
class wordsortunder{
public:
	bool operator()(const word a,word another) const {
			return a.type>another.type;
	}
};
class wostringsort{
public:
	bool operator()(const word a,word another) const {
			return a.surface>another.surface;
	}
};
class jyunsort{
	public:
	bool operator()(const pair<double,word> a,pair<double,word> another) const {
		return a.first>another.first;
	}
};

enum hinsi{
	ERO,
	KANDOUSHI,
	FUKUSHI,
	KEIYOUDOUSHI,
	KEIYOUSHI,
	RENTAISHI,
	MEISHI,
	DOUSHI,
	JYODOUSHI,
	JYOSI,
	KANTOUSHI,
	KIGOU,
	SETSUZOKUSHI,
	SETTOUSHI,
	NASHI,
	EOS,
	HIJIRITSU,
	KUTEN,
	TOUTEN,
};
enum imiornot{
	IMI=1,
	NOIMI=0,
};

enum katsuyou{
	MIZEN=1,
	RENYO=2,
	KIHON=3,
	RENTAI=4,
	KATEI=5,
	MEIREI=6,
	KATSUYOUNASHI,
	RENYOTE,
	IONBIN,
	HATUONBIN,
	SOKUONBIN,
	BUNGOKIHON,
};
enum mesisi_syousai{
	KOYU=1,
	FUTSU=2,
};

enum nandan{
	SONOTA,
	GODAN,
	ITIDAN,
	SIMONI,
	KAHEN,
	SAHEN,
	DA,
	IDAN,
};
int meishicode(string s){
	if(s.find("�ŗL����")!=string::npos){return KOYU;}
	return FUTSU;
}

int nandancode(string s){
	if(s.find("����,")!=string::npos){return SONOTA;}
	if(s.find("�ܒi")!=string::npos){return GODAN;}
	else if(s.find("��i")!=string::npos){return ITIDAN;}
	else if(s.find("�J��")!=string::npos){return KAHEN;}
	else if(s.find("�T��")!=string::npos){return SAHEN;}
	else if(s.find("�C�i")!=string::npos){return IDAN;}
	else if(s.find("�A�E�I�i")!=string::npos){return IDAN;}
	return SONOTA;
}

bool ifyokuderu(string togo){
	if(togo.find("����")!=string::npos)return "����";
	if(togo.find("����")!=string::npos||togo.find("��")!=string::npos||togo.find("����")!=string::npos||
		togo.find("����")!=string::npos||togo.find("����")!=string::npos||togo.find("�L��")!=string::npos||
		togo.find("�Ȃ�")!=string::npos||togo.find("�Ȃ�")!=string::npos
		){
			return true;
	}else{
		return false;
	}
}
string changehito(string togo){
	if(togo.find("���Ȃ�")!=string::npos||togo.find("�N")!=string::npos||togo.find("���܂�")!=string::npos){
			return "��";
	}else if(togo.find("�킽��")!=string::npos||togo.find("��")!=string::npos||togo.find("����")!=string::npos||
		togo.find("�ڂ�")!=string::npos||togo.find("�l")!=string::npos||togo.find("��")!=string::npos||
		togo.find("����")!=string::npos||togo.find("��y")!=string::npos||togo.find("�킪�͂�")!=string::npos||
		togo.find("��y")!=string::npos||togo.find("��")!=string::npos
		){
		return "���Ȃ�";
	}else if(togo.find("����")!=string::npos){
		return "����";
	}else if(togo.find("�ǂ�")!=string::npos){
		return "����";
	}else if(togo.find("�N")!=string::npos||togo.find("����")!=string::npos){
		return "��";
	}else{
		return togo;
	}
}
bool ifgimonsi(string togo){
	if(togo.find("�Ȃ�")!=string::npos||togo.find("��")!=string::npos||togo.find("�ǂ�")!=string::npos||
		togo.find("�ǂ�")!=string::npos||togo.find("�ǂ���")!=string::npos||togo.find("�ǂȂ�")!=string::npos||
		togo.find("�ǂ�")!=string::npos||togo.find("����")!=string::npos||togo.find("����")!=string::npos||
		togo.find("������")!=string::npos||togo.find("�ǂ�")!=string::npos
		){
			return true;
	}else{
		return false;
	}

}
int typecode(string s){
	if(s.find("�񎩗�")!=string::npos){return HIJIRITSU;}
	if(s.find("�ڔ�")!=string::npos){return HIJIRITSU;}
	if(s.find("����")!=string::npos){return HIJIRITSU;}
	if(s.find("����,")!=string::npos){return MEISHI;}
	else if(s.find("�A�̎�,")!=string::npos){return RENTAISHI;}
	else if(s.find("�ړ���,")!=string::npos){return SETTOUSHI;}
	else if(s.find("������,")!=string::npos){return KANDOUSHI;}
	else if(s.find("�`�e��,")!=string::npos){return KEIYOUSHI;}
	else if(s.find("������,")!=string::npos){return JYODOUSHI;}
	else if(s.find("�`�e����,")!=string::npos){return KEIYOUDOUSHI;}
	else if(s.find("����,")!=string::npos){return DOUSHI;}
	else if(s.find("����,")!=string::npos){return FUKUSHI;}
	else if(s.find("����,")!=string::npos){return JYOSI;}
	else if(s.find("�Ǔ_,")!=string::npos){return TOUTEN;}
	else if(s.find("��_,")!=string::npos){return KUTEN;}
	else if(s.find("�I,")!=string::npos){return KUTEN;}
	else if(s.find("�H,")!=string::npos){return KUTEN;}
	else if(s.find("�ԓ�")!=string::npos){return KANTOUSHI;}
	else if(s.find("�L��,")!=string::npos){return KIGOU;}
	else if(s.find("�ڑ���,")!=string::npos){return SETSUZOKUSHI;}
	else if(s.find("EOS")!=string::npos){return EOS;}
	return ERO;
}
int ifnaiyo(int type){
	switch(type){
		case MEISHI:
		case KEIYOUSHI:
		case KEIYOUDOUSHI:
		case DOUSHI:
		//case FUKUSHI:
		case KUTEN:
		case TOUTEN:
		case KANDOUSHI:
		case HIJIRITSU:
		//case JYOSHI:
			return IMI;
			break;
		case -1:
			return -1;
			break;
		default:
			return NOIMI;
	}
}
int katsuyoucode(string s){
	if(s.find("���R")!=string::npos){return MIZEN;}
	else if(s.find("�A�p�e�ڑ�")!=string::npos){return RENYOTE;}
	else if(s.find("�C����")!=string::npos){return IONBIN;}
	else if(s.find("�A�p�^�ڑ�")!=string::npos){
		if((s.find("�^�s")!=string::npos)||(s.find("�n�s")!=string::npos)||(s.find("���s")!=string::npos))return SOKUONBIN;
		else if((s.find("�i�s")!=string::npos)||(s.find("�}�s")!=string::npos)||(s.find("�o�s")!=string::npos))return HATUONBIN;
	}
	else if(s.find("�A�p")!=string::npos){return RENYO;}
	else if(s.find("�����{")!=string::npos){return BUNGOKIHON;}
	else if(s.find("��{")!=string::npos){return KIHON;}
	else if(s.find("�A��")!=string::npos){return RENTAI;}
	else if(s.find("����")!=string::npos){return KATEI;}
	else if(s.find("����")!=string::npos){return MEIREI;}
	return KATSUYOUNASHI;
}

string genkei_GODAN(string s,string f){
	string ret;
	
	//�i��,�i���ו���1,�i���ו���2,�i���ו���3,���p�`,���p�^,���`,�ǂ�,����	
	rep(i,6){
		int b=f.find(",");
		f.replace(0,b+1,"");
		int e=f.find(",");
		ret=f.substr(0,e);
	}
	return ret;	
}
struct dictionary{
	string word;
	double weight;
};

word changetokihon(word w,const char *feature){
	word wo0=w;
	if(wo0.type==MEISHI)return wo0;
	if(wo0.type==DOUSHI||wo0.type==KEIYOUDOUSHI||wo0.type==KEIYOUSHI)wo0.surface=genkei_GODAN(wo0.surface,feature);
	else wo0.surface=henkei(wo0.surface,wo0.nandan,wo0.katsuyou_type,KIHON);
	wo0.katsuyou_type=KIHON;
	return wo0;
}
multimap<word,pair<int,double>,wostringsort > dic;//���t�̗���<��ށA�d��>�B
vector<vector<word> > claster;//�b��̎�ނ��Ƃ̒P�꒠
map<word,pair<double,double>,wostringsort> Odic;//���t���Ƃ̍��v�̏d��,���t�̈ʒu�X�R�A(�ʒu/��)�����������ĈӖ��Ȃ��Ȃ��ĂȂ��H�H
#define GAKUSYU_HANI 1
vector<word> sentfordic[GAKUSYU_HANI];
int dc=0;
int clasternum=-1;

void sentence_learning(){
	word search;
	dc-=GAKUSYU_HANI/2;
	vector<int> cl;
	clasternum++;
	rep(pl,GAKUSYU_HANI){
		int dist=min(pl,GAKUSYU_HANI-pl);
		double kihongakuritu=1.0;
		rep(i,sentfordic[(dc+pl)%GAKUSYU_HANI].size()){//�Ӗ��̂��錾�t���ƂɁ@�֘A�Â�
			search=sentfordic[(dc+pl)%GAKUSYU_HANI][i];
			double gakuritu=kihongakuritu;
			if(search.type==MEISHI&&search.meishi_type==KOYU)gakuritu*=5.0;
			if(search.type==TOUTEN)continue;
			if(search.ifnaiyo==IMI&&search.type!=TOUTEN&&search.type!=KUTEN){
				multimap<word, pair<int,double> ,wostringsort>::iterator itr;
				dic.insert(make_pair(search,make_pair( clasternum ,gakuritu )) );
				if(claster.size()<clasternum+1){
					vector<word> clp;
					clp.push_back(search);
					claster.push_back(clp);
				}else{
					claster[clasternum].push_back(search);
				}
				map<word, pair<double,double>,wostringsort>::iterator oitr;
				oitr=Odic.find(search);
				if(oitr==Odic.end()){
					Odic.insert( make_pair(search,make_pair(gakuritu,i/sentfordic[(dc+pl)%GAKUSYU_HANI].size() ) ) );
				}else{
					oitr->second.first+=gakuritu;
					double gnum=oitr->second.first/gakuritu;
					oitr->second.second=(oitr->second.second*gnum  +  i/sentfordic[(dc+pl)%GAKUSYU_HANI].size() )/(gnum+1);
				}
			}
		}
	}
	dc+=(GAKUSYU_HANI/2)+1;sentfordic[dc%GAKUSYU_HANI].clear();
	
}

void sentence_learning_online(){//���ꂳ�����̊֐��Ƃ܂�������������Ȃ��H(�H)
	if(sentfordic[(dc+0)%1].size()<=1)return;
	word search;
	dc-=GAKUSYU_HANI/2;
	vector<int> cl;
	clasternum++;
	rep(pl,GAKUSYU_HANI){
		int dist=min(pl,GAKUSYU_HANI-pl);
		double kihongakuritu=1.0;
		rep(i,sentfordic[(dc+pl)%GAKUSYU_HANI].size()){
			search=sentfordic[(dc+pl)%GAKUSYU_HANI][i];
			double gakuritu=kihongakuritu;
			if(search.type==MEISHI&&search.meishi_type==KOYU)gakuritu*=5.0;
			if(search.surface=="�B")continue;
			if(search.ifnaiyo==IMI&&search.type!=TOUTEN&&search.type!=KUTEN){
				multimap<word, pair<int,double> ,wostringsort>::iterator itr;
				dic.insert(make_pair(search,make_pair( clasternum ,gakuritu )) );
				if(claster.size()<clasternum+1){
					vector<word> clp;
					clp.push_back(search);
					claster.push_back(clp);
				}else{
					claster[clasternum].push_back(search);
				}
				map<word, pair<double,double>,wostringsort>::iterator oitr;
				oitr=Odic.find(search);
				if(oitr==Odic.end()){
					Odic.insert( make_pair(search,make_pair(gakuritu,i/sentfordic[(dc+pl)%GAKUSYU_HANI].size() ) ) );
				}else{
					oitr->second.first+=gakuritu;
					double gnum=oitr->second.first/gakuritu;
					oitr->second.second=(oitr->second.second*gnum  +  i/sentfordic[(dc+pl)%GAKUSYU_HANI].size() )/(gnum+1);
				}
			}
		}
	}
	dc+=(GAKUSYU_HANI/2)+1;sentfordic[dc%GAKUSYU_HANI].clear();
	
}

vector<word> sentence_output(vector<word> keywords){
	int putoutsu=2+(int)( rand() * (2)/(1.0 + RAND_MAX) );
	map<int,double> omomis;
	vector<word> ret;
	rep(r,keywords.size()){
		keywords[r].surface=changehito(keywords[r].surface);
		if(keywords[r].type==KUTEN||keywords[r].type==TOUTEN||ifgimonsi(keywords[r].surface)||ifyokuderu(keywords[r].surface)){
			continue;
		}
		ret.push_back(keywords[r]);		
		multimap<word, pair<int,double>,wostringsort >::iterator itr;
		map<word,pair<double,double>,wostringsort>::iterator oitr;
		itr = dic.find(keywords[r]);
		oitr=Odic.find(keywords[r]);
		double wordO=10;
		if(oitr!=Odic.end()){wordO=oitr->second.first;}
		int cou = (int)dic.count(keywords[r]);
		for (int i = 0; i < cou; i++) {
			pair<int,double> k= itr->second;
			if(k.first>=claster.size())continue;
			int w=omomis[k.first];
			omomis[k.first]=w+ (k.second/wordO);
			itr++;
		}
	}
	vector<int> omomi_max_syurui;
	double omomi_max_omomi=0.0;
	map<int,double>::iterator itr;
	itr= omomis.begin();
	rep(r,omomis.size()){
		double omomiritsu=itr->second;
		int syurui=itr->first;
		if(omomiritsu>omomi_max_omomi){
			omomi_max_syurui.clear();
			omomi_max_syurui.push_back(syurui);
			omomi_max_omomi=omomiritsu;
		}
		else if(omomiritsu==omomi_max_omomi){
			omomi_max_syurui.push_back(syurui);
			omomi_max_omomi=omomiritsu;
		}
		itr++;
	}
	if(omomi_max_syurui.size()==0){
		sort(ret.begin(),ret.end(),wordsort() );
		return ret;
	}else{
		keywords.erase( unique(keywords.begin(),keywords.end()) ,keywords.end() );
		vector<word> lan;
		vector<word> rep1;
		
		rep(i,omomi_max_syurui.size()){
			lan.insert(lan.end(), claster[omomi_max_syurui[i]].begin(), claster[omomi_max_syurui[i]].end());
		}
		int s=0+(int)( rand() * lan.size()/(1.0 + RAND_MAX) );
		if(lan[s].type==MEISHI){
			rep1.push_back(lan[s]);
			rep(k,putoutsu){
				if(s+k+1<lan.size()){
					if(find(rep1.begin(),rep1.end(),lan[s+k+1]) == rep1.end() )rep1.push_back(lan[s+k+1]);}
			}
		}else if(lan[s].type==DOUSHI){
			rep(k,putoutsu){
				if(s-putoutsu*2+k>=0){
					if(find(rep1.begin(),rep1.end(),lan[s-putoutsu+k]) == rep1.end() )rep1.push_back(lan[s-putoutsu+k]);}
			}
			rep1.push_back(lan[s]);
		}else {
			rep(k,putoutsu){
				if(s-putoutsu+k>=0){
					if(find(rep1.begin(),rep1.end(),lan[s-putoutsu+k]) == rep1.end() )rep1.push_back(lan[s-putoutsu+k]);}
			}
			rep1.push_back(lan[s]);
			rep(k,putoutsu){
				if(s+k+1<lan.size()){
					if(find(rep1.begin(),rep1.end(),lan[s+k+1]) == rep1.end() )rep1.push_back(lan[s+k+1]);}
			}
		}
	
		rep1.erase(unique(rep1.begin(),rep1.end()) , rep1.end());
			
		return rep1;
		
	}
}

string henkei(string s,int nandan,int bef, int aft){
	string ret;
	ret=s;
	if(nandan==SONOTA)return s;
	if(bef==-1)return s;
	else if(nandan==GODAN){
		string sample[14][5]={
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
			{"��","��","��","��","��"},
		};
		string fin=ret;
		fin.replace(0,fin.size()-2,"");
		int num=0;
		rep(i,14)rep(j,5){
			string h=sample[i][j];
			if(h==fin)num=i;
		}
		if(bef==MIZEN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENYO){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENYOTE||bef==HATUONBIN||bef==SOKUONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==IONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KIHON){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENTAI){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KATEI){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==MEIREI){ret.replace(ret.size()-2,ret.size(),"");}
		//if(bef==MEIREI){ret.replace("����","\0");
		string plus;
		//if((s.find("�^�s")!=string::npos)||(s.find("�n�s")!=string::npos)||(s.find("���s")!=string::npos))return SOKUONBIN;
		//else if((s.find("�i�s")!=string::npos)||(s.find("�}�s")!=string::npos)||(s.find("�o�s")!=string::npos))return HATUONBIN;
		//������ӕ��@������
		if(aft==MIZEN){plus=sample[num][0];}
		if(aft==RENYO){plus=sample[num][1];}
		if(aft==RENYOTE||aft==HATUONBIN||aft==SOKUONBIN||aft==IONBIN){
			if(num==5||num==13||num==12)plus="��";
			else if(s.find("�s��")!=string::npos){plus="��";}
			else if(num==7||num==9||num==10)plus="��";
			else if(num==1||num==2) plus="��";
			else plus=plus=sample[num][1];
		}
		if(aft==KIHON){plus=sample[num][2];}
		if(aft==RENTAI){plus=sample[num][2];}
		if(aft==KATEI){plus=sample[num][3];}
		if(aft==MEIREI){plus=sample[num][3];}
		ret+=plus;
	}
	else if(nandan==IDAN){
		if(bef==MIZEN){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==RENYO){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==IONBIN){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==RENYOTE||bef==HATUONBIN||bef==SOKUONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KIHON){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENTAI){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KATEI){ret.replace(ret.size()-4,ret.size(),"");}
		//if(bef==MEIREI){ret.replace("����","\0");
		string plus;
		if(aft==MIZEN){plus="����";}
		//if(){plus="����";}
		//�������̋�ʃ_��
		if(aft==IONBIN||aft==HATUONBIN||aft==SOKUONBIN||aft==RENYO){plus="��";}
		if(aft==RENYOTE){plus="����";}
		if(aft==KIHON){plus="��";}
		if(aft==RENTAI){plus="��";}
		if(aft==KATEI){plus="����";}
		if(aft==MEIREI){plus="";}
		ret+=plus;
	}
	else if(nandan==ITIDAN){
		if(bef==KIHON){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENTAI){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KATEI){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==MEIREI){ret.replace(ret.size()-2,ret.size(),"");}
		string plus;
		if(aft==KIHON){plus="��";}
		if(aft==RENTAI){plus="��";}
		if(aft==KATEI){plus="��";}
		if(aft==MEIREI){plus="��";}
		ret+=plus;
	}
	else if(nandan==KAHEN){
		if(bef==MIZEN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENYO){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENYOTE||bef==HATUONBIN||bef==SOKUONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==IONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KIHON){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==RENTAI){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==KATEI){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==MEIREI){ret.replace(ret.size()-4,ret.size(),"");}
		string plus;
		if(aft==MIZEN){plus="��";}
		if(aft==RENYO){plus="��";}
		if(aft==RENYOTE){plus="��";}
		if(aft==IONBIN||aft==HATUONBIN||aft==SOKUONBIN){plus="��";}
		if(aft==KIHON){plus="����";}
		if(aft==RENTAI){plus="����";}
		if(aft==KATEI){plus="����";}
		if(aft==MEIREI){plus="����";}
		ret+=plus;
	}
	else if(nandan==SAHEN){
		if(bef==MIZEN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENYO){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENYOTE||bef==HATUONBIN||bef==SOKUONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==IONBIN){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==KIHON){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==BUNGOKIHON){ret.replace(ret.size()-2,ret.size(),"");}
		if(bef==RENTAI){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==KATEI){ret.replace(ret.size()-4,ret.size(),"");}
		if(bef==MEIREI){ret.replace(ret.size()-4,ret.size(),"");}
		string plus;
		if(aft==MIZEN){plus="��";}
		if(aft==RENYO){plus="��";}///////�����@���i-�Ȃ��A-�悤�j�A���i-���j�A���i-����A-���j
		if(aft==RENYOTE||aft==HATUONBIN||aft==SOKUONBIN){plus="��";}
		if(aft==IONBIN){plus="��";}
		if(aft==KIHON){plus="����";}
		if(aft==RENTAI){plus="����";}
		if(aft==KATEI){plus="����";}
		if(aft==MEIREI){plus="����";}
		ret+=plus;
	}
	return ret;
}
int ifhijiritu(string s){
	if(s.find("�񎩗�")!=string::npos){return 1;}
	if(s.find("����")!=string::npos){return 1;}
	if(s.find("�ڔ�")!=string::npos){return 1;}
	return 0;
}

vector< pair<string,int> > blockwords[20][20];
//��Ǔ_�p�x
int allTOUTEN=0;
int KANTAN=0;
string MIKE_BUNGO_REPLY(char player[],bool ifonline){
	if(ifonline){
		FILE *onlinef;
		onlinef=fopen("text\\���Ȃ��̃I�E��.txt","a");
		fprintf(onlinef,"%s\n",player);
		fclose(onlinef);
	}
  /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////    �������甽��   /////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////
	  vector<string> output;
	  vector<word> wo;
			MeCab::Tagger *tagger = MeCab::createTagger("-d MeCab\\dic\\ipadic");
			const MeCab::Node* node = tagger->parseToNode(player);
			bool end=false;
			int cn=0;
			for (; node; ){
				cn++;
				wo2=wo1;
				wo1=wo0;
				char tan[1000];
				strncpy(tan,node->surface,node->length);
				tan[node->length]='\0';
				wo0.surface.clear();
				wo0.surface=tan;
				wo0.type=typecode(node->feature);
				
				if(end)break;
				if(cn>1&&wo0.type==EOS){
					end=true;
				}
				wo0.katsuyou_type=katsuyoucode(node->feature);
				wo0.ifnaiyo=ifnaiyo(wo0.type);
				if(wo0.type==MEISHI)wo0.meishi_type=meishicode(node->feature);
				else wo0.meishi_type=0;
				wo0.nandan=nandancode(node->feature);

				
				if(wo0.type==MEISHI){
					while(1){
						if(node->next){
						node = node->next;
						word st0;
						char tan[1000];
						strncpy(tan,node->surface,node->length);
						tan[node->length]='\0';
						st0.surface.clear();
						st0.surface=tan;
						st0.type=typecode(node->feature);
						st0.katsuyou_type=katsuyoucode(node->feature);
						st0.ifnaiyo=ifnaiyo(st0.type);
						if(st0.type==MEISHI)st0.meishi_type=meishicode(node->feature);
						else st0.meishi_type=0;
						wo0.nandan=nandancode(node->feature);
						if(st0.type!=MEISHI&&st0.type!=HIJIRITSU)break;
						if(wo0.type==HIJIRITSU)wo0.type=st0.type;
						wo0.surface+=st0.surface;
						}else break;
						
					}
					
					word put;
					put=changetokihon(wo0,node->feature);
					wo.push_back(put);
					if(ifonline)sentfordic[dc%GAKUSYU_HANI].push_back(changetokihon(wo0,node->feature));//�����p
					continue;
				}else if(ifonline){//////////�����w�K
					if(wo0.ifnaiyo==IMI&&wo0.type!=HIJIRITSU){
						sentfordic[dc%GAKUSYU_HANI].push_back(changetokihon(wo0,node->feature));//�����p
					}
					
					if(ifonline){
						if(wo0.type==KUTEN||wo0.type==TOUTEN||wo0.type==EOS){
								if(wo0.surface=="�B"||wo0.surface=="�I"||wo0.surface=="!"){
									allTOUTEN++;
									if(wo0.surface!="�B")KANTAN++;
									}
							}
						if(wo0.ifnaiyo==NOIMI&&wo0.type!=KUTEN&&wo0.type!=TOUTEN){
									while(1){
										if(node->next){
										node = node->next;
										word st0;
										char tan[1000];
										strncpy(tan,node->surface,node->length);
										tan[node->length]='\0';
										st0.surface.clear();
										st0.surface=tan;
										st0.type=typecode(node->feature);
										st0.katsuyou_type=katsuyoucode(node->feature);
										st0.ifnaiyo=ifnaiyo(st0.type);
										if(st0.type==MEISHI)st0.meishi_type=meishicode(node->feature);
										else st0.meishi_type=0;
										wo0.nandan=nandancode(node->feature);
										if(st0.ifnaiyo==IMI)break;
										wo0.surface+=st0.surface;
										
										}else break;
										
									}
								
									
									wo0.type=NASHI;
								}	
							if(wo0.type!=NASHI){
								if(node->next){
									node = node->next;
									while(ifhijiritu(node->feature) ){
												char tan[1000];string plus;
												strncpy(tan,node->surface,node->length);
												tan[node->length]='\0';
												plus.clear();
												plus=tan;
												wo0.surface+=plus;
												if(!node->next){break;}
												node=node->next;
									}
								}
							}
							if(wo1.type==ERO){cn=0;continue;}			
							//�Ӗ��L�薳���f�[�^���Ƃ̓ǂݍ���
							if(wo2.katsuyou_type!=KATSUYOUNASHI){
								string check=wo1.surface.substr(0,2);string check2;
								if(wo1.surface.size()>=4)check2=wo1.surface.substr(2,4);
								if(check=="��"){
									wo2.katsuyou_type=RENYOTE;
								}
								if(check=="��"&&check2!="��"){
									wo2.katsuyou_type=HATUONBIN;
								}
								blockwords[wo2.type][wo0.type].push_back(make_pair(wo1.surface,wo2.katsuyou_type));
							}
							if(wo2.type!=KEIYOUDOUSHI&&wo2.type!=KEIYOUSHI&&wo2.type!=DOUSHI&&wo2.type!=JYODOUSHI){
								blockwords[wo2.type][wo0.type].push_back(make_pair(wo1.surface,wo2.katsuyou_type));
							}
				
					}
				}

				if(wo0.ifnaiyo==IMI&&wo0.type!=HIJIRITSU){
					word put;
					put=changetokihon(wo0,node->feature);
					wo.push_back(put);
					
				}
				if(node->next){
					node=node->next;
				}
			}
			
			if(tagger)delete tagger;
  /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////    �֘A����P��𒊏o�B   /////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////

			vector<word> learned=sentence_output(wo);//�ꉞ�Bwo�̎��_�Ŋ�{�`�ɂȂ��Ă���͂��Ȃ̂ŋC�ɂ��Ȃ��Ă悢�H
			wo.clear();
			learned.erase( unique(learned.begin(),learned.end() ) , learned.end() );
			wo=learned;
			if(ifonline){
				sentence_learning_online();
			}
		


			rep(i,wo.size()){		
				if(i==wo.size()-1){
					
								if(blockwords[wo[i].type][KUTEN].size() ){
									int ran=0+(int)(rand()%blockwords[wo[i].type][KUTEN].size() );
									pair<string,int> p=blockwords[wo[i].type][KUTEN][ran];
									string str=p.first;int aft=p.second;
									
								if(str.find("null")!=string::npos){
									output.push_back(henkei(wo[wo.size()-1].surface,wo[wo.size()-1].nandan,wo[wo.size()-1].katsuyou_type,KIHON));
								}else{
									output.push_back(henkei(wo[i].surface,wo[i].nandan,wo[i].katsuyou_type,aft) );
										output.push_back(str);
								}
										
										
								}else{
										output.push_back(henkei(wo[wo.size()-1].surface,wo[wo.size()-1].nandan,wo[wo.size()-1].katsuyou_type,KIHON));
								}
					
				}else{
					if(wo[i].type!=FUKUSHI){
						if(blockwords[wo[i].type][wo[i+1].type].size() ){
							int ran=0+(int)(rand()%blockwords[wo[i].type][wo[i+1].type].size() );
							pair<string,int> p=blockwords[wo[i].type][wo[i+1].type][ran];
							string str=p.first;
							int aft=p.second;
							if(str.find("null")!=string::npos){
								if(wo[i+1].type==MEISHI){aft=RENTAI;}
								if(wo[i+1].type==DOUSHI){aft=RENYO;}
								if(wo[i+1].type==JYODOUSHI){aft=RENYOTE;}
								output.push_back(henkei(wo[i].surface,wo[i].nandan,wo[i].katsuyou_type,aft) );
							}else{
								wo[i].surface=henkei(wo[i].surface,wo[i].nandan,wo[i].katsuyou_type,aft);
								output.push_back( wo[i].surface);
								if(wo[i].type==DOUSHI)if(aft==RENYO||aft==RENYOTE||aft==IONBIN||aft==HATUONBIN||aft==SOKUONBIN){
									string check = wo[i].surface.substr(wo[i].surface.size()-2,wo[i].surface.size());
									
									if(check=="��"){
										check=str.substr(0,2);
										if(check=="��"){
											str.replace(0,2,"��");
										}
										if(check=="��"){
											str.replace(0,2,"��");
										}
									}else{
										check=str.substr(0,2);
										if(check=="��"){
											str.replace(0,2,"��");
										}
										if(check=="��"){
											str.replace(0,2,"��");
										}
									}
									/*if(check=="��"){
										check=str.substr(0,2);
										if(check=="��"){
											str.replace(0,2,"��");
										}
										if(check=="��"){
											str.replace(0,2,"��");
										}
									}*/
								}
								if(wo[i].type==KANDOUSHI){
									string p="�A";
									output.push_back(p);
								}
								output.push_back(str);
							}
						}else{
						output.push_back(wo[i].surface);
						}
					}else{
						output.push_back(wo[i].surface);
					}
				}
			}
			
				
				

				string togo;
				rep(i,output.size()){
					if(output[i].find("�B")!=string::npos)break;
					togo+=output[i];
					
				}
				if(ifgimonsi(togo)){
					string las=togo.substr(togo.size()-2,togo.size());
					if(las!="��"){togo+="��";}
					togo+="�H";
				}else{
					double probability = (double)(KANTAN)/(double)(allTOUTEN); // �m��
					srand((unsigned)time(NULL)); // �����̏�����

					if ( (double)rand()/RAND_MAX < probability ) {
						togo+="�I";	
					}else{
						togo+="�B";
					}
				}
				output.clear();
	return togo;
}

void MIKE_BUNGO_DICTIONALY(char filename[]){
  char input[10024];
  FILE *text;
  if( (text=fopen(filename,"r")) == NULL){
	  printf("�����f�[�^���ǂݍ��߂܂���c\n");
  }else{
	  while(1){
		  if(fgets(input,10024,text)==NULL)break;
		  double stop=0.0;
		  MeCab::Tagger *tagger = MeCab::createTagger("-d MeCab\\dic\\ipadic");
			const MeCab::Node* node = tagger->parseToNode(input);
			int cn=0;
			for (; node; ){
			cn++;
			wo2=wo1;
			wo1=wo0;
			char tan[1000];
			strncpy(tan,node->surface,node->length);
			tan[node->length]='\0';
			wo0.surface.clear();
			wo0.surface=tan;
			wo0.type=typecode(node->feature);
			if(wo0.type==EOS&&cn>1)break;
			wo0.katsuyou_type=katsuyoucode(node->feature);
			wo0.ifnaiyo=ifnaiyo(wo0.type);
			if(wo0.type==KUTEN||wo0.type==TOUTEN||wo0.type==EOS){
				sentence_learning();
			}
			if(wo0.type==MEISHI)wo0.meishi_type=meishicode(node->feature);
			else wo0.meishi_type=0;
			wo0.nandan=nandancode(node->feature);
		
			/////////////////
			if(wo0.type==MEISHI){
					while(1){
						if(node->next){
						node = node->next;
						word st0;
						char tan[1000];
						strncpy(tan,node->surface,node->length);
						tan[node->length]='\0';
						st0.surface.clear();
						st0.surface=tan;
						st0.type=typecode(node->feature);
						st0.katsuyou_type=katsuyoucode(node->feature);
						st0.ifnaiyo=ifnaiyo(st0.type);
						if(st0.type==MEISHI)st0.meishi_type=meishicode(node->feature);
						else st0.meishi_type=0;
						wo0.nandan=nandancode(node->feature);
						if(st0.type!=MEISHI&&st0.type!=HIJIRITSU)break;
						if(wo0.type==HIJIRITSU)wo0.type=st0.type;
						wo0.surface+=st0.surface;
						
						}else break;
						
					}
					sentfordic[dc%GAKUSYU_HANI].push_back(changetokihon(wo0,node->feature));//�����p
					continue;
			}
			if(wo0.type!=NASHI&&wo0.ifnaiyo==IMI&&wo0.type!=HIJIRITSU){
				sentfordic[dc%GAKUSYU_HANI].push_back(changetokihon(wo0,node->feature));//�����p
			}//�����ɒǉ�
			
			if(wo0.type!=NASHI){
				if(node)node = node->next;
			}
			if(wo1.type==ERO){cn=0;continue;}			
			}
		delete tagger;
	  }
	  fclose(text);
  }

 
  return ;
}
int MIKE_BUNGO_LEARN(char filename[]){
	
  char input[10024];
  FILE *text;
  if( (text=fopen(filename,"r")) == NULL){
	  printf("���t�p�f�[�^���ǂݍ��߂܂���c\n");
  }else{
	  while(1){
		  if(fgets(input,10024,text)==NULL)break;
		  double stop=0.0;
		  MeCab::Tagger *tagger =MeCab::createTagger("-d MeCab\\dic\\ipadic ");
			const MeCab::Node* node = tagger->parseToNode(input);
			int cn=0;
			for (; node; ){
				if(wo0.type==KUTEN||wo0.type==TOUTEN||wo0.type==EOS){
				if(wo0.surface=="�B"||wo0.surface=="�I"||wo0.surface=="!"){
					allTOUTEN++;
					if(wo0.surface!="�B")KANTAN++;
					}
				}
			cn++;
			wo2=wo1;
			wo1=wo0;
			char tan[1000];
			strncpy(tan,node->surface,node->length);
			tan[node->length]='\0';
			wo0.surface.clear();
			wo0.surface=tan;
			wo0.type=typecode(node->feature);
			if(wo0.type==EOS&&cn>1)break;
			wo0.katsuyou_type=katsuyoucode(node->feature);
			wo0.ifnaiyo=ifnaiyo(wo0.type);
			if(wo0.type==MEISHI)wo0.meishi_type=meishicode(node->feature);
			else wo0.meishi_type=0;
			wo0.nandan=nandancode(node->feature);
			
				
				
			
			if(wo0.ifnaiyo==NOIMI&&wo0.type!=KUTEN&&wo0.type!=TOUTEN){
					
					while(1){
						if(node->next){
						node = node->next;
						word st0;
						char tan[1000];
						strncpy(tan,node->surface,node->length);
						tan[node->length]='\0';
						st0.surface.clear();
						st0.surface=tan;
						st0.type=typecode(node->feature);
						st0.katsuyou_type=katsuyoucode(node->feature);
						st0.ifnaiyo=ifnaiyo(st0.type);
						if(st0.type==MEISHI)st0.meishi_type=meishicode(node->feature);
						else st0.meishi_type=0;
						wo0.nandan=nandancode(node->feature);
						if(st0.ifnaiyo==IMI)break;
						wo0.surface+=st0.surface;
						
						}else break;
						
					}
				
					wo0.type=NASHI;
				}	
			
			if(cn>2&&wo0.ifnaiyo==IMI&&wo1.ifnaiyo==IMI){
				wo0.surface="null";
				wo0.ifnaiyo=NOIMI;
				wo0.type=NASHI;
			}else{
			}
			if(wo0.type!=NASHI){
				node = node->next;
				while(ifhijiritu(node->feature) ){
							char tan[1000];string plus;
							strncpy(tan,node->surface,node->length);
							tan[node->length]='\0';
							plus.clear();
							plus=tan;
							wo0.surface+=plus;
							node=node->next;
				}
			}
			if(wo1.type==ERO){cn=0;continue;}			
			//�Ӗ��L�薳���f�[�^���Ƃ̓ǂݍ���
			if(wo1.ifnaiyo==IMI||cn<2){continue;}
			if(wo2.katsuyou_type!=KATSUYOUNASHI){
				string check=wo1.surface.substr(0,2);string check2;
				if(wo1.surface.size()>=4)check2=wo1.surface.substr(2,4);
				if(check=="��"){
					wo2.katsuyou_type=RENYOTE;
				}
				if(check=="��"&&check2!="��"){
					wo2.katsuyou_type=HATUONBIN;
				}
				blockwords[wo2.type][wo0.type].push_back(make_pair(wo1.surface,wo2.katsuyou_type));
			}
			if(wo2.type!=KEIYOUDOUSHI&&wo2.type!=KEIYOUSHI&&wo2.type!=DOUSHI&&wo2.type!=JYODOUSHI){
				blockwords[wo2.type][wo0.type].push_back(make_pair(wo1.surface,wo2.katsuyou_type));
			}
			
		}

		delete tagger;
		 
	  }
	  }if(text)fclose(text);
	
  return 0;
}

vector<string> FIND_YFILES(){
	vector<string> ret;
	WIN32_FIND_DATA ffd;
	 HANDLE h = FindFirstFile("data\\*.markov", &ffd);
	 if ( h != INVALID_HANDLE_VALUE ) {
	 do {
		 string r;
		 r=ffd.cFileName;
		 r.replace(r.find("."),r.size(),"");
		 ret.push_back(r);
		 } while ( FindNextFile(h, &ffd) );
	 FindClose(h);
	 }
	 return ret;
}

vector<string> FIND_FILES(){
	vector<string> ret;
	WIN32_FIND_DATA ffd;
	 HANDLE h = FindFirstFile("text\\*.txt", &ffd);
	 if ( h != INVALID_HANDLE_VALUE ) {
	 do {
		 string r;
		 r=ffd.cFileName;
		 r.replace(r.find("."),r.size(),"");
		 ret.push_back(r);
		 } while ( FindNextFile(h, &ffd) );
	 FindClose(h);
	 }
	 return ret;
}