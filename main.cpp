//Jogo de Damas


#include <iostream>

using namespace std;

enum pecas{BRANCA, PRETA};

class Peca{
    int cor;
public:
    Peca(){
    }
    Peca(int c): cor(c){
    }
    int getCor(){
      return cor;
    }
    void setCor(int val){
      cor = val;
    }
};

class Rainha: private Peca{
public:
};
class Vazio: private Peca{
};
class Comum: private Peca{
};
class Tabuleiro{
  int proxJogador;
public:
  Peca tabuleiro[8][8];
    Tabuleiro():proxJogador(0){
            for(int i =0; i<8; i++){
                for(int j= 0; j<8; j++){
                    if((i == 0 || i ==2)  && !(j%2)){
                        Peca a(PRETA);
                        tabuleiro[i][j] = a;
                        continue;
                    }
                    if(i == 1 && (j%2)){
                      Peca b(PRETA);
                      tabuleiro[i][j] = b;
                      continue;
                    }
                    if((i == 5 || i ==7)  && (j%2)){
                      Peca c(BRANCA);
                      tabuleiro[i][j] = c;
                      continue;
                    }
                    if(i == 6 && !(j%2)){
                      Peca d(BRANCA);
                      tabuleiro[i][j] = d;
                      continue;
                    }
                    else{
                      Peca v(3);
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
        for(int j = 0; j<8; j++){
          cout << tabuleiro[i][j].getCor() << "|";
        }
        cout <<endl << "----------------" << endl;
      }
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