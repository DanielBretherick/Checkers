//Jogo de Damas
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

#define OPEN_RED "\033[1;31m"
#define OPEN_GREEN "\033[1;32m"

#define CLOSE_COLOR "\033[0m"



using namespace std;

enum cores {VERDE, VERMELHA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letrasCores[4] = {'g', 'r', 'G', 'R'};

class Peca;
class Comum;
class Rainha;

class Tabuleiro;

class Peca{
protected:
  static int numVermelha, numVerde;
  tiposPeca tipo;
  cores cor;
public:
  //Construtor
  Peca(tiposPeca tipo, cores cor): tipo(tipo), cor(cor){
    if(cor == VERDE)
      numVerde++;
    if(cor == VERMELHA)
      numVermelha++;
  }
  //Construtor Default
  Peca(): tipo(VAZIO), cor(NEUTRO) {
  }
  // Destrutor
  ~Peca() {
    if(cor == VERDE)
      numVerde--;
    if(cor== VERMELHA)
      numVermelha--;
  }
  cores getCor(){
    return cor;
  }
  tiposPeca getTipo() {return tipo;}
  static int getNumVerde() { return numVerde; }
  static int getNumVermelha() { return numVermelha; }
  virtual bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {return false;};
  virtual bool verificarPodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tab) {return false; }
};

int Peca::numVermelha = 0;
int Peca::numVerde = 0;

class Rainha: public Peca {
public:
  Rainha(cores cor): Peca(RAINHA, cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb);
};
class Comum: public Peca{
public:
  Comum(cores cor): Peca(COMUM, cor) {}
  Comum(const Comum &c): Peca(c.tipo, c.cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb);
  bool verificarPodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tab) {
    int sinalL = (lf-li)/abs(lf-li);
    int sinalC = (cf-ci)/abs(cf-ci);
    if(tab[li+sinalL][ci+sinalC] == )
   }


};

class Tabuleiro{
  cores proxJogador;
  Peca *tabuleiro[8][8];
public:
  Tabuleiro(): tabuleiro{NULL}, proxJogador(VERDE) {
    for(int i =0; i<8; i++){
      for(int j= 0; j<8; j++) {
        if((j == 0 || j ==2)  && (i%2)) {
          Comum *pA = new Comum(VERMELHA);
          tabuleiro[j][i] = pA;
        }
        if(j == 1 && !(i%2)){
          Comum *pB = new Comum(VERMELHA);
          tabuleiro[j][i] = pB;
        }
        if((j == 5 || j ==7)  && !(i%2)) {
          Comum *pC = new Comum(VERDE);
          tabuleiro[j][i] = pC;
        }
        if(j == 6 && (i%2)) {
          Comum *pD = new Comum(VERDE);
          tabuleiro[j][i] = pD;
        }
      }
    }
  }
  ~Tabuleiro(){
  }
  friend class Comum;
  friend class Rainha;
  void imprimirTabuleiro(){
    for(int i = 0; i<8; i++){
      cout << 8-i << "  ";
      for(int j = 0; j<8; j++) {
        if(tabuleiro[i][j] == NULL)
          cout << "  ";
        else {
          cores cor = tabuleiro[i][j]->getCor();
          cout << (cor == VERDE ? OPEN_GREEN : OPEN_RED);
          cout << " " << (tabuleiro[i][j]->getTipo() == RAINHA ? letrasCores[cor+2] : letrasCores[cor]) ;
          cout << CLOSE_COLOR;
        }
        if(j+1 < 8)
          cout << " |";
      }
      if(i+1<8)
        cout <<endl << "   -------------------------------" << endl;
      else
        cout << '\n' <<endl;
    }
    cout << "    ";
    for(int k=0; k<8; k++)
      cout << letrasColunas[k] << "   ";
    cout << endl << endl;
  }

  void transformarCoordenadas(int &li, char &ci, int &lf, char &cf){
    int i = count(letrasColunas.begin(), letrasColunas.end(), ci);
    int j = count(letrasColunas.begin(), letrasColunas.end(), cf);
    if(i == 0 || j == 0)
      cout << "\n*** --- Letra invalida --\n- ***"<< endl;
    else {
      for(int j = 0; j<8; j++){
        if(ci == letrasColunas[j])
          ci = j;
        if(cf == letrasColunas[j])
          cf = j;
      }
      li = 8 - li;
      lf = 8 - lf;
      ci = (int)ci;
      cf = (int)cf;
    }
  }

  bool verificarVitoria() {
    return (Peca::getNumVerde() == 0 || Peca::getNumVermelha() == 0) ? true : false;
  }

  void transformarRainha(int l, int c) {
    if(l == 0 && tabuleiro[l][c]->getCor() == VERDE ) { 
      Rainha *pG = new Rainha(VERDE);
      delete tabuleiro[l][c];
      tabuleiro[l][c] = pG;
    }
    if(l == 7 && tabuleiro[l][c]->getCor() == VERMELHA ) { 
      Rainha *pR = new Rainha(VERMELHA);
      delete tabuleiro[l][c];
      tabuleiro[l][c] = pR;
    }
  }

  void solicitarJogada() {
    int li, lf;
    char cci, ccf;
    cout << "\n********** TURNO DO JOGADOR " << letrasCores[proxJogador+2] << " **********\n" << endl;
    cout << "Escolha a posição da peça a ser movida: ";
    cin >> li >> cci;
    
    if(cci >= 'a' && cci <= 'z') {
      cci+= ('A'-'a');
    }

    cout << "Escolha o destino do movimento: ";
    cin >> lf >> ccf;
    cout << endl;
    
    if(ccf >= 'a' && ccf <= 'z') {
      ccf+= ('A'-'a');
    }

    jogar(li, cci, lf, ccf);
  }

  void jogar(int li, char cci, int lf, char ccf) {
    transformarCoordenadas(li, cci, lf, ccf);
    int ci = (int)cci;
    int cf = (int)ccf;
    if(!verificarJogada(li,ci,lf,cf)) {
      cout << "!!!!!!!!! Insira outra jogada !!!!!!!!!\n" << endl;
      solicitarJogada();
    }
    else{
      tabuleiro[lf][cf] = tabuleiro[li][ci];
      tabuleiro[li][ci] = NULL;

      int sinalL = (lf-li)/abs((lf-li));
      int sinalC = (cf-ci)/abs((cf-ci));

      if(tabuleiro[lf-sinalL][cf-sinalC] != NULL) {
        delete tabuleiro[lf-sinalL][cf-sinalC];
        tabuleiro[lf-sinalL][cf-sinalC] = NULL;
      }
      transformarRainha(lf, cf);
      imprimirTabuleiro();
      proxJogador = (proxJogador == VERDE) ? VERMELHA : VERDE;

      if(verificarVitoria()) {
        cout << "----------- Vitoria do jogador " << (Peca::getNumVerde() == 0 ? letrasCores[3] : letrasCores[2] );
        cout << " -----------" << endl;
      }
      else
        solicitarJogada();
    }
   }

  bool verificarJogada(int li, int ci, int lf, int cf) {
    if(li > 8 || ci > 8 || lf > 8 || cf > 8 || li < 0 || ci < 0 || lf < 0 || cf < 0) {
      cout << "\n*** --- Posicao fora do Tabuleiro --- ***\n" << endl;
      return false;
    }
    else {
      if(tabuleiro[li][ci] == NULL){
        cout << "\n*** --- Posição sem peças --- ***\n" << endl;
        return false;
        }
      else
        if(tabuleiro[li][ci]->getCor() != proxJogador){
          cout << "\n*** --- Peça de outro jogador --- ***\n" << endl;
          return false;
          }
    }
    return tabuleiro[li][ci]->verificarDestino(li, ci, lf, cf, *this);
  }
  void jogadaComputador(){
    int lf;
    int cf;
    //1. Escolher posicao
      //1.a percorrer tabuleiro vendo possibilidades de comer
      for(int l =0; l< 8; l++){
        for(int c =0; c<8; c++){
          if(tabuleiro[l][c] != NULL && tabuleiro[l][c]->getCor() == VERDE){
            tabuleiro[l][c]->verificarPodeComer(l, c, lf, cf, *this);
          }
        }
      }
    //2. Passas posicao para o verificarJogada

  }
};

bool Comum::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
  int dl = li -lf;
  int dc = cf -ci;
  if(abs(dl) != abs(dc)) { 
    cout << "\n*** --- Movimento não foi diagonal --- ***\n" << endl;
    return false;
  }
  if((dl<0 && (cor == VERDE))  || (dl > 0 && (cor == VERMELHA)) ) {
    cout << "\n*** --- Indo para tras --- ***\n" << endl; 
    return false;
  }
  if(tb.tabuleiro[lf][cf] != NULL) {
    cout << "\n*** --- Posicao ocupada --- ***\n" << endl;
    return false;
  }
  if(abs(dl) == 1 && abs(dc) == 1) {return true;}
  if(abs(dl) == 2 && abs(dc) == 2) {
    int sinalC = dc/abs(dc);
    int sinalL = -dl/abs(dl);
    if(tb.tabuleiro[li+sinalL][ci+sinalC] == NULL) {
      cout << "\n*** --- Nao pode pular duas, espaco vazio --- ***\n" << endl;
      return false;
    }
    else{
      Peca *aux = tb.tabuleiro[li+sinalL][ci+sinalC];
      if(aux->getCor() == cor){
        cout << "\n*** --- Nao pode comer a própria peca --- ***\n" << endl;
        return false;
      }
      return true;
    }
  }
  else{
    cout << "\n*** --- Mais de duas casas andadas --- ***\n" << endl;
    return false;
  }
}

bool Rainha::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
    int dl = lf -li;
    int dc = cf -ci;
    if(abs(dl)!=abs(dc)) {
      cout << "\n*** --- Movimento não foi diagonal --- ***\n"<< endl;
      return false;
    }
    if(tb.tabuleiro[lf][cf] != NULL) {
      cout << "\n*** --- Posição ocupada --- ***\n"<< endl;
      return false;
    }
    if(abs(dl) > 1 && abs(dc) > 1) {
      int sinalC = dc/abs(dc);
      int sinalL = dl/abs(dl);
      for(int l=li+sinalL,c=ci+sinalC ; l!=(lf-sinalL) ; ) {
        if(tb.tabuleiro[l][c] != NULL) {
          cout << "\n*** --- O caminho não está livre --- ***\n" << endl;
          return false;
        }
        l+=sinalL;
        c+=sinalC;
      }
      if(tb.tabuleiro[lf-sinalL][cf-sinalC] != NULL) {
        if(tb.tabuleiro[lf-sinalL][cf-sinalC]->getCor() == cor) {
          cout << "\n*** --- Não pode pular a própria peça --- ***\n";
          return false;
        }
      }
    }
    return true;
  }

int main(){

  Tabuleiro tab;
  tab.imprimirTabuleiro();
  tab.solicitarJogada();
  

  return 0;
}
