#include <iostream> // Trabalho feito por Gustavo Coelho e João Vitor Custódio na disciplina de Algoritmos e Programação II
//#include <time.h>
using namespace std;

struct Arma
{
	string nome;
	int min{};
	int max{};
};

struct Jogador
{
	int hp;
	int x;
	int y;
	Arma arma;
};

struct Inimigo
{
	string nome;
	int hp{};
	Arma arma;
};

struct Bloco {
	bool bloqueado = false;
	bool tem_inimigo = false;
	Inimigo* ini{};
};

struct Mapa {
	int a;
	int l;
	Bloco** pp_bloco;
};

struct Fase
{
	string nome;
	Mapa mapa{};
	int num_inimigos;
	Inimigo* inimigos;
};

Mapa CriarMapa(int altura, int largura) {
	Mapa mapa;
	mapa.a = altura;
	mapa.l = largura;
	
	mapa.pp_bloco = new Bloco* [altura];
	for (int i = 0; i < altura; i++) {
		mapa.pp_bloco[i] = new Bloco [largura];
	}
	
	int r;
	for (int i = 0; i < altura; i++) {
		for (int j = 0; j < largura; j++) {
			r = rand() % 10;
			if (r < 2) {
				mapa.pp_bloco[i][j].bloqueado = true;
			}
		}
	}
	return mapa;
}

Fase CriarFase(int numInimigos, Inimigo* inimigos, int alturaMapa, int larguraMapa) {
	Fase fase;
	fase.mapa = CriarMapa(alturaMapa, larguraMapa);
	fase.inimigos = inimigos;

	int r_linha;
	int r_coluna;
	
	for (int i = 0; i < numInimigos;) {
		r_linha = rand() % alturaMapa;
		r_coluna = rand() % larguraMapa;
		
		if (fase.mapa.pp_bloco[r_linha][r_coluna].bloqueado == false && fase.mapa.pp_bloco[r_linha][r_coluna].tem_inimigo == false) {
			fase.mapa.pp_bloco[r_linha][r_coluna].tem_inimigo = true;
			fase.mapa.pp_bloco[r_linha][r_coluna].ini = &fase.inimigos[i];
			i++;
		}
	}
	return fase;
}

void exibirMapa(Fase fase, Jogador jogador) {
	while (fase.mapa.pp_bloco[jogador.x][jogador.y].bloqueado == true || fase.mapa.pp_bloco[jogador.x][jogador.y].tem_inimigo == true) {
		jogador.x = rand() % fase.mapa.a;
		jogador.y = rand() % fase.mapa.l;
	}
	for (int i = 0; i < fase.mapa.a; i++) {
		cout << "\n";
		for (int j = 0; j < fase.mapa.l; j++) {
			if (i == jogador.x && j == jogador.y) {
				cout << "  |Jogador|  ";
			}
			else if (fase.mapa.pp_bloco[i][j].bloqueado == true) {
				cout << "  |*Bloco*|  ";
			}
			else if (fase.mapa.pp_bloco[i][j].tem_inimigo == true) {
				cout << "  |Inimigo|  ";
			}
			else {
				cout << "  |       |  ";
			}
		}
		cout << "\n";
	}
}

void movimentar() {

}

int main()
{
	srand(static_cast<unsigned>(time(NULL)));

	int altura = rand() % 5 + 4;
	int largura = rand() % 5 + 4;
	int numInimigos = rand() % 5 + 4;

	Arma arma[10];
	arma[0] = { "espada", 5, 10 };
	arma[1] = { "machado", 3, 12 };
	arma[2] = { "chicote", 1, 15 };
	arma[3] = { "alabarda", 10, 20 };
	arma[4] = { "nunchaku", 7, 7 };
	arma[5] = { "panela", 1, 2 };
	arma[6] = { "foice", 5, 30 };
	arma[7] = { "tridente", 3, 33 };
	arma[8] = { "???", 50, 100 };
	arma[9] = { "Zenith", 200, 200 };

	Inimigo inimigos[10];
	inimigos[0] = { "goblin", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[1] = { "esqueleto", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[2] = { "lobo", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[3] = { "zumbi", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[4] = { "ogro", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[5] = { "hipogrifo", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[6] = { "cabra", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[7] = { "troll", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[8] = { "aranha", rand() % 21 + 10, arma[rand() % 10] };
	inimigos[9] = { "Cthulhu", 50, arma[8] };
	
	Inimigo* p_inimigos = new Inimigo[numInimigos];
	for (int i = 0; i < numInimigos; i++) {
		p_inimigos[i] = inimigos[rand() % 10];
	}

	Fase fase = CriarFase(numInimigos, p_inimigos, altura, largura);
	Jogador jogador = { 500, rand() % altura, rand() % largura, arma[rand() % 10]};
	
	exibirMapa(fase, jogador);
}