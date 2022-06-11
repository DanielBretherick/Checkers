//Jogo de Damas


#include <iostream>

using namespace std;

enum cores {BRANCA, PRETA, NEUTRO};
enum tiposPeca { VAZIO, COMUM, RAINHA};
enum colunas {A, B, C, D, E, F, G, H};

char letrasColunas[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

class Peca{
  static int numPretas, numBrancas;
  tiposPeca status;
  cores cor;
  int x, y;
public:
    Peca():
      status(VAZIO) {
      
    }
    Peca(cores c): cor(c){
    }
    char getCor(){
      switch (cor) {
      case BRANCA:
        return 'O';
      case PRETA:
        return 'X';
      default:
        return ' ';
      }
    }
    void setCor(cores val){
      cor = val;
    }
};

class Rainha: private Peca{
};
class Vazio: private Peca{
};
class Comum: private Peca{
};


class Tabuleiro{
  int proxJogador;
public:
  Peca tabuleiro[8][8];
    Tabuleiro(): proxJogador(0){
            for(int i =0; i<8; i++){
                for(int j= 0; j<8; j++) {
                    if((i == 0 || i ==2)  && (j%2)){
                        Peca a(PRETA);
                        tabuleiro[i][j] = a;
                        continue;
                    }
                    if(i == 1 && !(j%2)){
                      Peca b(PRETA);
                      tabuleiro[i][j] = b;
                      continue;
                    }
                    if((i == 5 || i ==7)  && !(j%2)){
                      Peca c(BRANCA);
                      tabuleiro[i][j] = c;
                      continue;
                    }
                    if(i == 6 && (j%2)){
                      Peca d(BRANCA);
                      tabuleiro[i][j] = d;
                      continue;
                    }
                    else{
                      Peca v(NEUTRO);
                      tabuleiro[i][j] = v;
                      continue;
                    }
                }
            }
    }
    ~Tabuleiro(){
    }
    void imprimirTabuleiro(){
      for(int i = 0; i<8; i++){
        cout << 8-i << "  ";
        for(int j = 0; j<8; j++){
          cout << " " << tabuleiro[i][j].getCor();
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
      cout << endl;
    }
    Tabuleiro(const Tabuleiro &s){
    } //construtor por cópia
    int verificarJogada(int xi, int yi, int xf, int yf){
      if(xi >= 8 || yi >= 8 || xf >= 8 || yf >= 8 || xi < 0 || yi < 0 || xf < 0 || yf < 0){
        cout << "Jogada Invalida!!!" << endl;
        return 0;
      }
    }

};

int main(){
  Peca a(BRANCA);
  Tabuleiro tab;
  tab.imprimirTabuleiro();
  return 0;
}