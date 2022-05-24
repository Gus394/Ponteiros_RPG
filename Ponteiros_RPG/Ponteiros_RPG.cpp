#include <iostream> // Trabalho feito por Gustavo Coelho e João Vitor Custódio na disciplina de Algoritmos e Programação II
using namespace std;

struct Arma {
	string nome;
	int min;
	int max;
};

struct Jogador {
	int hp;
	int x;
	int y;
	Arma arma;
};

struct Inimigo {
	string nome;
	int hp;
	Arma arma;
};

struct Bloco {
	bool bloqueado = false;
	bool tem_inimigo = false;
	bool verificado = false;
	bool chegada = false;
	Inimigo* ini;
};

struct Mapa {
	int a;
	int l;
	bool tem_caminho = false;
	Bloco** pp_bloco = nullptr;
};

struct Fase {
	string nome;
	Mapa mapa;
	int num_inimigos;
	Inimigo* inimigos;
};

Mapa CriarMapa(int altura, int largura) {
	Mapa mapa;
	mapa.a = altura;
	mapa.l = largura;

	delete [] mapa.pp_bloco;
	
	mapa.pp_bloco = new Bloco* [altura];
	mapa.pp_bloco[0] = new Bloco [altura * largura];
	for (int i = 1; i < altura; i++) {
		mapa.pp_bloco[i] = mapa.pp_bloco[0] + i * largura;
	}
	
	int r_linha;
	int r_coluna;

	for (int i = 0; i < altura * largura / 2;) {
		r_linha = rand() % altura;
		r_coluna = rand() % largura;
		
		if ((r_linha == altura - 1 && r_coluna == largura - 1) || (r_linha == 0 && r_coluna == 0)) {
			
		}
		else if (mapa.pp_bloco[r_linha][r_coluna].bloqueado == false) {
			mapa.pp_bloco[r_linha][r_coluna].bloqueado = true;
			i++;
		}
	}
	mapa.pp_bloco[altura - 1][largura - 1].chegada = true;
	return mapa;
}

Mapa r_chegada(Mapa mapa, int x = 0, int y = 0) {
	if (mapa.pp_bloco[x][y].chegada == true) {
		mapa.tem_caminho = true;
	}
	else if (y + 1 < mapa.l && mapa.pp_bloco[x][y + 1].verificado == false && mapa.pp_bloco[x][y + 1].bloqueado == false) {
		mapa.pp_bloco[x][y + 1].verificado = true;
		return r_chegada(mapa, x, y + 1);
	}
	else if (x + 1 < mapa.a && mapa.pp_bloco[x + 1][y].verificado == false && mapa.pp_bloco[x + 1][y].bloqueado == false) {
		mapa.pp_bloco[x + 1][y].verificado = true;
		return r_chegada(mapa, x + 1, y);
	}
	else if (x - 1 >= 0 && mapa.pp_bloco[x - 1][y].verificado == false && mapa.pp_bloco[x - 1][y].bloqueado == false) {
		mapa.pp_bloco[x - 1][y].verificado = true;
		return r_chegada(mapa, x - 1, y);
	}
	else if (y - 1 >= 0 && mapa.pp_bloco[x][y - 1].bloqueado == false) {
		mapa.pp_bloco[x][y - 1].verificado = true;
		return r_chegada(mapa, x, y - 1);
	}
	
	return mapa;
}

Fase CriarFase(int numInimigos, Inimigo* inimigos, int alturaMapa, int larguraMapa) {
	Fase fase = {"sem nome", CriarMapa(alturaMapa, larguraMapa), numInimigos, inimigos};

	int r_linha;
	int r_coluna;
	
	for (int i = 0; i < numInimigos;) {
		r_linha = rand() % alturaMapa;
		r_coluna = rand() % larguraMapa;
		
		if (fase.mapa.pp_bloco[r_linha][r_coluna].bloqueado == false && fase.mapa.pp_bloco[r_linha][r_coluna].tem_inimigo == false && (r_linha != 0 && r_coluna != 0) && (r_linha != alturaMapa - 1 && r_coluna != larguraMapa - 1)) { // !
			fase.mapa.pp_bloco[r_linha][r_coluna].tem_inimigo = true;
			fase.mapa.pp_bloco[r_linha][r_coluna].ini = &fase.inimigos[i];
			i++;
		}
	}
	return fase;
}

void exibirMapa(Fase fase, Jogador jogador) {
	for (int i = 0; i < fase.mapa.a; i++) {
		cout << "\n";
		for (int j = 0; j < fase.mapa.l; j++) {
			if (i == jogador.x && j == jogador.y && fase.mapa.pp_bloco[i][j].tem_inimigo == true) {
				cout << "  |Combate|  ";
			}
			else if (i == fase.mapa.a - 1 && j == fase.mapa.l - 1)
				cout << "  |Chegada|  ";
			else if (i == jogador.x && j == jogador.y) {
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

template <typename AGRESSOR, typename ALVO>
int ataque(AGRESSOR x, ALVO y) {
	return y.hp - (rand() % (x.arma.max + 1 - x.arma.min) + x.arma.min);
}

Jogador combate(Jogador jogador, Fase fase) {
	Inimigo inimigo = *fase.mapa.pp_bloco[jogador.x][jogador.y].ini;
	cout << "\n Iniciando combate com: " << inimigo.nome << " (HP: " << inimigo.hp << "). "; system("pause");
	
	while (inimigo.hp > 0 && jogador.hp > 0) {
		
		inimigo.hp = ataque(jogador, inimigo);
		jogador.hp = ataque(inimigo, jogador);

		cout << "\n Jogador atacou com " << jogador.arma.nome << " (" << jogador.arma.min << "-" << jogador.arma.max << "): "
			 << inimigo.nome << " ficou com " << inimigo.hp << " de vida.\n";
		cout << " Oponente " << inimigo.nome << " atacou com " << inimigo.arma.nome << " (" << inimigo.arma.min << "-" << inimigo.arma.max << "):"
			 << " jogador ficou com " << jogador.hp << " de vida.\n";
	}
	cout << "\n ";
	system("pause");
	
	return jogador;
}

Jogador movimentar(Fase fase, Jogador jogador) {
	int x = jogador.x;
	int y = jogador.y;
	
	cout << "\n Utilize 'wasd' para se mover ou 'l' para desistir. Inimigos restantes: " << fase.num_inimigos << ".\n";
	char movimento;
	cin >> movimento;

	switch (movimento)
	{
	case 'w':
		x--;
		break;
	
	case 'a':
		y--;
		break;

	case 's':
		x++;
		break;

	case 'd':
		y++;
		break;

	case 'l':
		jogador.hp = 0;
		break;

	default:
		break;
	}
	if (x < 0 || y < 0 || x == fase.mapa.a || y == fase.mapa.l) {
		cout << "Fique dentro dos limites do mapa. ";
		system("pause");
	}
	else if (fase.mapa.pp_bloco[x][y].bloqueado == true) {
		cout << "Caminho bloqueado, tente novamente. ";
		system("pause");
	}
	else {
		jogador.x = x;
		jogador.y = y;
	}
	return jogador;
}

int main()
{
	srand((time(NULL)));

	int altura = 7;
	int largura = 7;
	int numInimigos = rand() % 5 + 4;

	Arma arma[10];
	arma[0] = { "espada", 5, 10 };
	arma[1] = { "machado", 3, 12 };
	arma[2] = { "chicote", 1, 15 };
	arma[3] = { "alabarda", 10, 20 };
	arma[4] = { "nunchaku", 7, 7 };
	arma[5] = { "panela", 4, 5 };
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
	
	while (fase.mapa.tem_caminho == false) {
		fase = CriarFase(numInimigos, p_inimigos, altura, largura);
		fase.mapa = r_chegada(fase.mapa);
	}
	
	Jogador jogador = { 1000, 0, 0, arma[rand() % 10]};
	
	while (jogador.hp > 0 && fase.num_inimigos > 0) {
		system("CLS");
		exibirMapa(fase, jogador);
		
		if (fase.mapa.pp_bloco[jogador.x][jogador.y].tem_inimigo == true) {
			jogador = combate(jogador, fase);
			fase.num_inimigos--;
			fase.mapa.pp_bloco[jogador.x][jogador.y].tem_inimigo = false;
		}
		else {
			jogador = movimentar(fase, jogador);
		}
	}

	if (jogador.hp > 0) {
		cout << "\n Todos os inimigos foram derrotados. Bom trabalho.\n\n";
	}
	else {
		cout << "\n Morreu...\n\n";
	}
}