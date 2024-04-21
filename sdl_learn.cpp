﻿/*#include "Includs.h"
#include "MySDL.h"
#include "Sprite.h"


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");


    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create window
        SDL_Window* gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get window surface
            SDL_Surface* gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

	SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	//const char* backscreen_name = "backscreen.jpg";
	//SDL_Texture* back_texture = sdl.loadTexture(backscreen_name);
	std::string doomer_sprite = "doomer_split_16frames.png";
	//SDL_Surface* surf = SDL_LoadBMP("backscreen.bmp");
	//std::cout << surf->w;
    //sdl.draw(surf);

    Sprite spr(sdl.getRender());
    bool flag = spr.LoadFromFile(doomer_sprite, 16);
    size_t frame = 0;
    
    if (flag)
    {
        while (frame < 16)
        {
            //Clear screen
            SDL_SetRenderDrawColor(sdl.getRender(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(sdl.getRender());

            spr.renderer(0, 0, frame);
            frame++;
            Sleep(200);
            //Update screen
            SDL_RenderPresent(sdl.getRender());
        }
    }
    else
    {
        std::cout << "LoadFromFile error " << SDL_Error << std::endl;
    }
	
	return 0;
}*/




#include "Includs.h"
#include "MySDL.h"
#include "Sprite.h"
#include "InitError.h"

void animation(bool flag, Sprite spr, SDL sdl, size_t frame)
{
    if (flag)
    {
        while (frame < 16)
        {
            //Clear screen
            SDL_SetRenderDrawColor(sdl.getRender(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(sdl.getRender());

            spr.renderer(0, 0, frame);
            frame++;
            Sleep(200);
            //Update screen
            SDL_RenderPresent(sdl.getRender());
        }
    }
    else
    {
        std::cout << "LoadFromFile error " << SDL_Error << std::endl;
    }
}

// blank - холостой
// live - заряженный
// mind - рассудок; восполняется энергетиками и сигаретами
// amount - количество

class Member {
protected:
    int loses_count;

public:
    Member() { loses_count = 0; }
    virtual int get_loses_count() = 0;
    virtual void add_loses_count() = 0;
    virtual int get_hp() = 0;
};
class Player : public Member {
private:
    int mind;
    int hp;
    int loses_count;

public:
    Player() {
        hp = 0;
        mind = 100;
        loses_count = 0;
    }
    void receive_damage(bool is_hitted) {
        if (is_hitted) {
            hp--;
            mind -= 10;
        }
    }
    void healhp(bool energy_drink) {
        if (energy_drink) {
            hp++;
        }
    }
    void healmind(bool cigarettes) {
        if (cigarettes) {
            mind += 10;
        }
    }
    int get_hp() override { return hp; }
    void set_hp(int newhp) { hp = newhp; }
    int get_mind_player() { return mind; }
    int get_loses_count() override { return loses_count; }
    void add_loses_count() override { loses_count++; }
};
class Enemy : public Member {
private:
    int hp;
    int loses_count;

public:

    Enemy() {
        hp = 0;
        loses_count = 0;
    }
    void receive_damage(bool is_hitted) {
        if (is_hitted) {
            hp--;
        }
    }
    void heal(bool energy_drink) {
        if (energy_drink) {
            hp++;
        }
    }
    int get_hp() override { return hp; }
    int get_loses_count() override { return loses_count; }
    void set_hp(int newhp) { hp = newhp; }
    void add_loses_count() override { loses_count++; }
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create window
        SDL_Window* gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get window surface
            SDL_Surface* gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    std::string doomer_sprite = "doomer_split_16frames.png";


    Sprite spr(sdl.getRender());
    bool flag_spr = spr.LoadFromFile(doomer_sprite, 16);
    size_t frame = 0;

    int num_of_energydrinks = 0, num_of_cigarettes = 0; //randomreceiving = 0
    int choosen_bullet = 0;
    bool is_hitted = false;
    Player player;
    Enemy enemy;
    int choice1 = 0, choice2 = 0;
    bool turn_to_shoot = 1; // 1 - очередь игрока, 0 - очередь противника
    //int bullets = 0;
    int amount_of_bullets;
    std::vector<int> gunstore;
    int amount_of_live_bullet_in_gunstore = 0;
    int choice = 0;
    int round = 1;
    bool flag = 1; // повторяется ли раунд (0) или переходит к следующему (1)
    std::cout << "Wanna play? 1 - yes, 0 - yes" << std::endl;
    std::cin >> choice1;
    if (choice1 == 1) {
        while (round < 4) { // цикл для того чтобы играть 3 раунда
            turn_to_shoot = 1;

            if (round == 1 && flag) {
                player.set_hp(2);
                enemy.set_hp(2);
            }
            else if (round == 2 && flag) {
                player.set_hp(4);
                enemy.set_hp(4);
            }
            else if (round == 3 && flag) {
                player.set_hp(6);
                enemy.set_hp(6);
            }
            std::cout << "Round " << round << std::endl;
            if (round == 1) {
                amount_of_bullets = 3;
                amount_of_live_bullet_in_gunstore = rand() % (amount_of_bullets - 1) + 1;
            }
            else {
                amount_of_bullets = rand() % (2 * round) + 4;
                amount_of_live_bullet_in_gunstore = rand() % (amount_of_bullets - 1) + 2;
            }
            // amount_of_bullets = rand() % (3 + 2*round); // общее количество пуль в
            // магазине может быть
            //  кол-во боевых зависит от общего кол-ва (всего 3 -> боевых может
            //  быть от 1 до 2)
            for (int i = 0; i < amount_of_bullets; i++) {
                gunstore.push_back(0);
            }
            for (int i = 0; i < amount_of_live_bullet_in_gunstore; i++) {
                gunstore[i] = 1;
            }

            auto iter = gunstore.begin();
            while (gunstore.size() > 0)
            { // цикл для игры внутри одного раунда
                std::cout << "The gun is loaded" << std::endl;
                // вывод магазина
                // system("cls");
                iter = gunstore.begin();
                for (int i = 0; i < gunstore.size(); i++) {
                    std::cout << gunstore[i] << " ";
                }
                std::cout << std::endl;
                if (turn_to_shoot) {
                    std::cout << "1. Shoot yourself" << std::endl;
                    std::cout << "2. Shoot enemy" << std::endl;
                    std::cout << "3. Heal hp or mind" << std::endl;
                    std::cout << "Enter your choice: ";
                    std::cin >> choice;
                    switch (choice) {
                    case 1:
                        choosen_bullet = rand() % gunstore.size();
                        is_hitted = gunstore[choosen_bullet];
                        if (gunstore.size() == 1) {
                            gunstore.clear();
                        }
                        else {
                            gunstore.erase(iter + choosen_bullet);
                        }
                        player.receive_damage(is_hitted);
                        if (is_hitted) {
                            animation(flag_spr, spr, sdl, frame);
                            std::cout << "Ты ебанутый там боевой патрон" << std::endl;
                            std::cout << "GOAAAAAAAAAAAAAAAAAAL" << std::endl;
                            std::cout << "Your current hp " << player.get_hp() << std::endl;
                            turn_to_shoot = 0;
                        }
                        else {
                            std::cout << "Повезло? там холостой" << std::endl;
                            turn_to_shoot = 1;
                        }
                        if (player.get_hp() == 0) {
                            std::cout << "You died" << std::endl;
                            player.add_loses_count();
                            flag = 1;
                            turn_to_shoot = 1;
                            break;
                        }
                        break;
                    case 2:
                        choosen_bullet = rand() % gunstore.size();
                        is_hitted = gunstore[choosen_bullet];
                        if (gunstore.size() == 1) {
                            gunstore.clear();
                        }
                        else {
                            gunstore.erase(iter + choosen_bullet);
                        }
                        enemy.receive_damage(is_hitted);
                        if (is_hitted) {
                            std::cout << "You hitted enemy" << std::endl;
                            std::cout << "Enemys current hp " << enemy.get_hp() << std::endl;
                            if (rand() % 2 == 0) {
                                std::cout << "You received 1 energy drink" << std::endl;
                                num_of_energydrinks++;
                            }
                            else {
                                std::cout << "You received 1 cigarette" << std::endl;
                                num_of_cigarettes++;
                            }
                            if (enemy.get_hp() == 0) {
                                std::cout << "Enemy loses" << std::endl;
                                enemy.add_loses_count();
                                flag = 1;
                                break;
                            }
                        }
                        else {
                            std::cout << "You missed" << std::endl;
                        }
                        turn_to_shoot = 0;
                        break;
                    case 3:
                        if (num_of_cigarettes > 0 || num_of_energydrinks > 0)
                        {
                            std::cout << "You have " << num_of_cigarettes << " cigarettes "
                                << "and " << num_of_energydrinks << " energy drinks"
                                << std::endl;
                            std::cout << "What do you want to heal? 1 - hp (drink energy "
                                "drink), 2 - mind (smoke cigarette)"
                                << std::endl;
                            std::cin >> choice2;
                        }
                        else
                        {
                            std::cout << "You don`t have enough cigarettes or energy drinks to heal" << std::endl;
                        }
                        if (choice2 == 1) {
                            if (num_of_energydrinks > 0) {
                                player.healhp(true);
                                num_of_energydrinks--;
                                std::cout << "You healed yourself" << std::endl;
                                std::cout << "You have " << num_of_energydrinks
                                    << " energy drinks left" << std::endl;
                                std::cout << "You have " << player.get_hp() << " hp left"
                                    << std::endl;
                            }
                            else
                                std::cout << "You don't have energy drinks" << std::endl;
                        }
                        else {
                            if (num_of_cigarettes > 0) {
                                player.healmind(true);
                                num_of_cigarettes--;
                                std::cout << "You healed yourself" << std::endl;
                                std::cout << "You have " << num_of_cigarettes
                                    << " cigarettes left" << std::endl;
                                std::cout << "You have " << player.get_mind_player()
                                    << " mind left" << std::endl;
                            }
                            else
                                std::cout << "You don't have cigarettes" << std::endl;
                        }
                        break;
                    }//////// end of switch

                }
                if (gunstore.size() > 0 && turn_to_shoot == 0) {
                    std::cout << "Enemy shoots" << std::endl;
                    iter = gunstore.begin();
                    choosen_bullet = rand() % gunstore.size();
                    is_hitted = gunstore[choosen_bullet];

                    amount_of_bullets = gunstore.size();
                    float count_live_bullets = 0;
                    for (int i = 0; i < gunstore.size(); ++i) {
                        if (gunstore[i] == 1)
                            count_live_bullets += 1;
                    }
                    if (gunstore.size() == 1) {
                        gunstore.clear();
                    }
                    else {
                        gunstore.erase(iter + choosen_bullet);
                    }
                    if (count_live_bullets / amount_of_bullets >= 0.5) {
                        player.receive_damage(is_hitted);
                        if (is_hitted) {
                            std::cout << "Ты получил в ебало" << std::endl;
                            std::cout << "Your current hp " << player.get_hp() << std::endl;
                            if (player.get_hp() == 0) {
                                std::cout << "You died" << std::endl;
                                player.add_loses_count();
                                flag = 1;
                                break;
                            }
                        }
                        else {
                            std::cout << "Player: Повезло? там холостой" << std::endl;
                        }
                        turn_to_shoot = 1;
                    }
                    else {
                        enemy.receive_damage(is_hitted);
                        if (is_hitted) {
                            std::cout << "Enemy: Блять я себя захуярил" << std::endl;
                            std::cout << "Enemys current hp " << enemy.get_hp() << std::endl;
                            turn_to_shoot = 1;
                        }
                        else {
                            std::cout << "Enemy: Ахахахахахахахах повезло холостой" << std::endl;
                            turn_to_shoot = 0;
                        }
                        if (enemy.get_hp() == 0) {
                            std::cout << "Enemy loses" << std::endl;
                            enemy.add_loses_count();
                            flag = 1;
                            break;
                        }
                    }
                }
                if (enemy.get_hp() == 0 || player.get_hp() == 0)
                {
                    break;
                }
            }
            round++;
            if (player.get_hp() > 0 && enemy.get_hp() > 0) {
                round--;
                flag = 0;
                std::cout << "You are out of bullets, but still alive" << std::endl;
                std::cout << "Round continues" << std::endl;
            }
            else {
                std::cout << "You are out of bullets" << std::endl;
                std::cout << "Round ends" << std::endl;
            }
        }
        std::cout << "You have " << player.get_loses_count() << " loses" << std::endl;
        std::cout << "Enemy has " << enemy.get_loses_count() << " loses" << std::endl;
    }
    return 0;
}



