#include "stdafx.h"
#include "Grid.h"
vector<Player> players(50);

Grid grid;

int main()
{	
	ofstream file;

	Player::randomizePlayers(players);
	players[0] = Player();
	Player::adjustAttributes(players);

	for (int i = 0; i < Player::ATTRIBUTES; i++)
		cout << players[0].attributes[i] << ' ';
	cout << endl;

	file.open("results.txt", ios::app);
	file << "Starting with  " << players.size() << " players" << endl;
	file.close();

	for(int gen = 1;; gen++)
	{
		cout << "generation " << gen << endl;
		Player::resetPlayersStats(players);
		for (int player = 0; player < players.size(); player++)
		{
			for (int game = 0; game < 3; game++)
			{
				grid.play(&players[player], 29);
			}
			cout << "player " << player << " finished. Fitness " << players[player].fitness() << " ttr " << players[player].TTR << endl;
			for (int i = 0; i < Player::ATTRIBUTES; i++)
				cout << players[player].attributes[i] << ' ';
			cout << endl;
		}
		Player::sortPlayers(players);
		Player::adjustAttributes(players);
		for (int i = 0; i < Player::ATTRIBUTES; i++)
			cout << players[0].attributes[i] << ' ';
		cout << endl;
		cout << "Best fitness " << players[0].fitness() << endl;
		file.open("results.txt", ios::app);
		file << "generation " << gen << " top fitness " << players[0].fitness() << endl;
		for (int i = 0; i < Player::ATTRIBUTES; i++)
			file << players[0].attributes[i] << ' ';
		file << endl;
		for(int i = 0; i < players.size(); i++)
			file << "player " << i << "fitness " << players[i].fitness() << endl;
		file.close();
	}
	return 9999;
}