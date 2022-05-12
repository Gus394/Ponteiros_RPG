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
	Inimigo* ini;
};

struct Mapa {
	int a;
	int l;
	Bloco** pp_bloco;
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
	
	mapa.pp_bloco = new Bloco* [altura];
	mapa.pp_bloco[0] = new Bloco [altura * largura];
	for (int i = 1; i < altura; i++) {
		mapa.pp_bloco[i] = mapa.pp_bloco[0] + i * largura;
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
	Fase fase = {"sem nome", CriarMapa(alturaMapa, larguraMapa), numInimigos, inimigos};

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
	for (int i = 0; i < fase.mapa.a; i++) {
		cout << "\n";
		for (int j = 0; j < fase.mapa.l; j++) {
			if (i == jogador.x && j == jogador.y && fase.mapa.pp_bloco[i][j].tem_inimigo == true) {
				cout << "  |Combate|  ";
			}
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

	int altura = rand() % 5 + 4;
	int largura = rand() % 5 + 4;
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
	Jogador jogador = { 1000, rand() % altura, rand() % largura, arma[rand() % 10]};

	while (fase.mapa.pp_bloco[jogador.x][jogador.y].bloqueado == true || fase.mapa.pp_bloco[jogador.x][jogador.y].tem_inimigo == true) {
		jogador.x = rand() % fase.mapa.a;
		jogador.y = rand() % fase.mapa.l;
	}
	
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