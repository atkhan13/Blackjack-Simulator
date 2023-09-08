#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <chrono>
#include <random>
using namespace std;

//global variables
enum Rank {ACE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum Type {CLUBS, DIAMONDS, HEARTS, SPADES};
double numLosses = 0.0;
double numWins = 0.0;


class Card
{
    public:
    //Card constructor takes in the rank and type
    Card(Rank r, Type t)
    {
        myRank = r;
        myType = t;
    }
    ~Card(){};
    //switch statement check the inpued rank enum and returns corresponding value (Ace is always displayed as 1)
    int getValue()
    {
        switch (myRank) {
            case ACE: return 1;
                break;
            case TWO: return 2;
                break;
            case THREE: return 3;
                break;
            case FOUR: return 4;
                break;
            case FIVE: return 5;
                break;
            case SIX: return 6;
                break;
            case SEVEN: return 7;
                break;
            case EIGHT: return 8;
                break;
            case NINE: return 9;
                break;
            case TEN: return 10;
                break;
            case JACK: return 10;
                break;
            case QUEEN: return 10;
                break;
            case KING: return 10;
                break;
        }
        return -1;
    }
    //Display function uses getValue() function above. If the value is 10 it then checks if a 10, K or Q should be returned.
    //It then uses a switch statement to check the type and return and builds a string to be outputted.
    void displayCard()
    {
        int r = getValue();
        string val = std::to_string(r);
        if(r == 10)
        {  
            if(myRank == TEN)
            {
                val = "10";
            }
            else if(myRank == JACK)
            {
                val = 'J';
            }
            else if(myRank == QUEEN)
            {
                val = 'Q';
            }
            else if(myRank == KING)
            {
                val = 'K';
            }
        }
        switch(myType)
        {
            case CLUBS: cout<<val<<'C';
                break;
            case DIAMONDS: cout<<val<<'D';
                break;
            case HEARTS: cout<<val<<'H';
                break;
            case SPADES: cout<<val<<'S';
                break;
        }
    }

    protected:
    Rank myRank;
    Type myType;
};

class Hand
{
    public:
    //Number of cards always starts as 0
    Hand()
    {
        numCards = 0;
    }
    //destructor calls the clear function to clear a hand after the game
    ~Hand()
    {
        clear();
    }
    //Returns the card vector which represents a hand. This is so the hand can be accessed in BlackJackGame
    vector<Card> getCards()
    {
        return myHand;
    }
    //returns number of cards needed in BlackJackGame
    int getNumCards()
    {
        return numCards;
    }
    //Adds a card to the end of a hand and increases number of cards by 1
    void add(Card to_add)
    {
        myHand.push_back(to_add);
        numCards++;
    }
    //Empties the vector representing the hand
    void clear()
    {
        myHand.clear();
    }
    //Gets the total. If there is an Ace, the number to be returned will be the total returned will be that which is the largest
    //or in the case of a bust, the total which avoids you getting a bust.
    int getTotal()
    {
        int total1 = 0;
        int total2 = 0;
        for(int i = 0; i < numCards; i++)
        {
            if(myHand[i].getValue() == 1)
            {
                total1 += 1;
                total2 += 11;
            }
            else
            {
                total1+= myHand[i].getValue();
                total2+= myHand[i].getValue();
            }
        }
        if(total1 == total2)
        {
            return total1;
        }
        else
        {
            if ((total1 > total2))
            {
                if(total1 <= 21)
                {
                    return total1;
                }
                return total2;
            }
            else
            {
                if(total2 <= 21)
                {
                    return total2;
                }
                return total1;
            }
        }
    }
    protected:
    vector<Card> myHand;
    int numCards;
};

class Deck
{
    public:
    //initialize number of cards to 52
    Deck()
    {
        num_cards = 52;
    }
    ~Deck(){}
    //returns number of cards in the deck to be used in BlackJackGame
    int getNumCards()
    {
        return num_cards;
    }
    //Returns deck to be used in BlackJackGame
    vector<Card> getDeck()
    {
        return deck;
    }
    //Clears a deck and returns number of cards back to 52. To be used in BlackJackGame
    void clear()
    {
        deck.clear();
        num_cards = 52;
    }
    //Goes through each type and rank and creates a card with that rank and type and adds it to the deck
    void populate()
    {
        Rank ranks[] = {Rank::ACE, Rank::TWO, Rank::THREE, Rank::FOUR, Rank::FIVE, Rank::SIX, Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING};
        Type types[] = {Type::CLUBS, Type::DIAMONDS, Type::HEARTS, Type::SPADES};
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 13; j++)
            {
                Card newCard = Card(ranks[j], types[i]);
                deck.push_back(newCard);
            }
        }

    }
    //uses a random seed and the shuffle function to randomly reorganize the elements in the populated deck
    void shuffle()
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    }
    //deals a card to the hand given and removes this card from the current deck
    Card deal()
    {
        Card dealtCard = deck[0];
        deck.erase(deck.begin());
        num_cards --;
        return dealtCard;
    }
    //'cheat' function which tricks the randomness of the deck when casino lose percentage is too high
    void trickDeck(int totalPlayer, Hand casino)
    { 
        //Check how much is needed to beat the player. 
        int numToGet = 0;
        if(totalPlayer == 21)
        {
            //if player got 21, then the casino also needs to get 21
            numToGet = 21 - casino.getTotal();
        }
        else
        {
            //if the amount is not 21, the casino only needs one more than the current total of the player
            numToGet = totalPlayer + 1 - casino.getTotal();
        }
        vector<int> indices_to_shuffle;
        int total_cas = casino.getTotal();
        //iterate through the deck and note down the index in the deck of cards we want to move to the front of the deck
        for(int i = 0; i < num_cards; i++)
        {
            if(numToGet == 0)
            {
                break;
            }
            // first we choose cards that will get the casino's hand to 16 and note which indices have these cards. 
            if(total_cas < 16)
            {
                if(deck[i].getValue() + total_cas <= 16)
                {
                    indices_to_shuffle.push_back(i);
                    numToGet -= deck[i].getValue();
                    total_cas += deck[i].getValue();
                }
            }
            /* Once the casino is right at 16, it can only draw one more time before it can no longer draw. This is following the 
            rules which states that the casino keeps drawins as long as its total is less or equal to 16. When the total is 16
            we then find the card which results in the casino getting higher than the amount needed to beat the player, but also
            not too high that it causes the casino to bust. It then nodes the index of which card will give this result and then
            breaks out of the loop since at this point we have noted all the indices of the cards we need to push to the font 
            of the deck to assure that we win */
            else if(numToGet - deck[i].getValue() <= 0 && deck[i].getValue() + total_cas <= 21)
            {
                indices_to_shuffle.push_back(i);
                break;
            }
        }
        /* We then have to move the cards we need to win to the front of the deck. To do this we iterate through the vector which
        contains the information of which index has the card we need. We then switch the card at the front for the card at the desired index
        and go through the deck switching the first x cards in the deck for the cards at the desired index. The amount we iterate through
        is equal to the number of cards we need to switch as indicated by the size of the vector containing the indices. */ 
        int size = indices_to_shuffle.size();
        for(int i = 0; i < size; i++)
        {
            Card tmp = deck[i];
            deck[i] = deck[indices_to_shuffle.at(i)];
            deck[indices_to_shuffle.at(i)] = tmp;
        }
    }
    protected:
    vector<Card> deck;
    int num_cards;
};
class AbstractPlayer
{
    public:
    virtual bool isDrawing() = 0;
    //isBusted takes in a hand, calculates the total and returns true if the total is greater than 21 or false otherwise
    bool isBusted(Hand myHand)
    {
        int total = myHand.getTotal();
        if(total > 21)
        {
            return true;
        }
        return false;
    }

};

class HumanPlayer : public AbstractPlayer
{
    public:
    HumanPlayer(){};
    ~HumanPlayer(){};
    //isDrawing for the HumanPlayer will ask the player if they want to draw or not and return true if yes and false otherwise
    bool isDrawing()
    {
        char answer = 'y';
        cout<<"Do you want to draw? (y/n): ";
        cin>>answer;
        return(answer == 'y' ? true : false);
    }
    //announce method which tells the use who won
    void announce(Hand casino, Hand player)
    {
        int totalCasino = casino.getTotal();
        int totalPlayer = player.getTotal();
        bool casinoBusted = isBusted(casino);
        bool playerBusted = isBusted(player);

        if(totalPlayer > totalCasino)
        {
            /*if the total of the player is larger than the casino we need to make sure the player has not busted. If not, then
            the player wins, but if yes then the casino wins as long as it has not busted as well. If it has busted as well then
            no one wins*/
            if(!playerBusted)
            {
                cout<<"Player wins"<<endl;
                numLosses += 1.0;
            }
            else
            {
                if(!casinoBusted)
                {
                    cout<<"Casino wins"<<endl;
                    numWins += 1.0;
                }
                else
                {
                    cout<<"Push: No one wins"<<endl;
                }
            }
        }
        else if(totalCasino > totalPlayer)
        {
            /*if the total of the casino is larger than the player we need to make sure the casino has not busted. If not, then
            the casino wins, but if yes then the player wins as long as it has not busted as well. If it has busted as well then
            no one wins.*/
            if(!casinoBusted)
            {
                cout<<"Casino wins"<<endl;
                numWins += 1.0;
            }
            else
            {
                if(!playerBusted)
                {
                    cout<<"Player wins"<<endl;
                    numLosses += 1.0;
                }
                else
                {
                    cout<<"Push: No one wins"<<endl;
                }
            }
        }
        else
        {
            cout<<"Push: No one wins"<<endl;
        }
        /*Note: each time the casino wins we add one to global variable numWins and each time player wins we add 1 to global 
        variable numLosses to keep track of winning right. Push is neither a win or a loss so we do not count it*/
    }
};

class ComputerPlayer : public AbstractPlayer
{
    public:
    ComputerPlayer(){}
    ~ComputerPlayer(){}
    //the total of the casino will need to be updated as the game progresses so a set method is used to update this variable
    void setTotalCasino(int total)
    {
        casinoTotal = total;
    }
    //Per the rules, as long as the casinos total is below or equal to 16, it will keep drawing
    bool isDrawing()
    {
        if(casinoTotal <= 16)
        {
            return true;
        }
        return false;
    }
    private:
    int casinoTotal;
};

class BlackJackGame
{
    public:
    BlackJackGame(){};
    ~BlackJackGame(){}
    void play()
    {
        /*Because each time a new round is played we have to re shuffle the deck (as what was said on the discussion board)
        we start by populating the deck and shuffling it*/
        m_deck.populate();
        m_deck.shuffle();

        //We create an instance of HumanPlayer and Hand which will be the players hand and deal it two cards
        HumanPlayer player;
        Hand player_hand;
        player_hand.add(m_deck.deal());
        player_hand.add(m_deck.deal());

        //We create an instance of Hand which will be the casinos hand and deal it one card
        Hand casino_hand;
        casino_hand.add(m_deck.deal());

        //Display the casinos cards and the players cards to the user
        cout<<"Casino: ";
        casino_hand.getCards()[0].displayCard();
        cout<<" ["<<casino_hand.getCards()[0].getValue()<<"]"<<endl;

        cout<<"Player:";
        player_hand.getCards()[0].displayCard();
        cout<<" ";
        player_hand.getCards()[1].displayCard();
        cout<<" ["<<player_hand.getTotal()<<"]"<<endl;

        //Check if the player wants to draw
        bool continueDrawing = player.isDrawing();

        //As long as the player wants to keep drawing we output the following piece of code
        while(continueDrawing)
        {
            //Deal a card to the players hand and output all the cards in the players hand to the user
            player_hand.add(m_deck.deal());
            int num = player_hand.getNumCards();
            cout<<"Player: ";
            for(int i = 0; i < num; i++)
            {
                player_hand.getCards()[i].displayCard();
                cout<<" ";
            }
            cout<<"["<<player_hand.getTotal()<<"]"<<endl;
            //If being dealt a card leads to a bust then the player cannot draw anymore so we break out of the loop
            if(player.isBusted(player_hand))
            {
                break;
            }
            else
            {
                //If the player can still draw (they have not busted) we ask if they would like to keep drawing
                continueDrawing = player.isDrawing();
            }
        }
        //If the player busts then the casino does not need to draw so we announce the casino as the winner and exit the play method
        if(player.isBusted(player_hand))
        {
            cout<<"Player busts."<<endl;
            player.announce(casino_hand, player_hand);
            return;
        }

        //If the casino can play, we first need to set the total of the casino 
        m_casino.setTotalCasino(casino_hand.getTotal());
        //Calculate the win rate. Since pushes are neither wins nor losses we do not count them when considering the win rate
        double winRate = 0.0;
        if(numWins != 0 || numLosses != 0)
        {
            winRate = numWins/(numLosses+numWins);
        }
        //If win rate is below 55% then we need to trick the deck to assure the casino wins. We call the trickDeck function
        //and play with this rigged deck.
        if(winRate < 0.55)
        {

            m_deck.trickDeck(player_hand.getTotal(), casino_hand);

        }

        //Check is casino would like to draw
        continueDrawing = m_casino.isDrawing();


        while(continueDrawing)
        {
            //Deal a card to the casino and display all the casino's cards to the user
            casino_hand.add(m_deck.deal());

            int num = casino_hand.getNumCards();
            cout<<"Casino: ";
            for(int i = 0; i < num; i ++)
            {
                casino_hand.getCards()[i].displayCard();
                cout<<" ";
            }
            cout<<"["<<casino_hand.getTotal()<<"]"<<endl;
            //If casino is busted it cannot draw anymore so we break out of for loop
            if(m_casino.isBusted(casino_hand))
            {
                cout<<"Casino busted."<<endl;
                break;
            }
            //Update total of the casino to figure out if it needs to keep drawing or not and check if it would like to draw
            m_casino.setTotalCasino(casino_hand.getTotal());
            continueDrawing = m_casino.isDrawing();
        }
        //Announce the winner of the game
        player.announce(casino_hand, player_hand);
        /*Because we create an instance of m_deck in the constructor, we need to clear the deck so that everytime play() gets
        called we repopulate and reshuffle so that we are playing with a new deck for every round. It is important to note
        that I did not call clear on the Hands of the casino or player because the destructor calls this function. Because
        an instance of Hand is created during play() and not in the constructor (like with the deck), the destructor will 
        get called at the end of every round (when play() finishes execution) so the Hand will be cleared. */
        m_deck.clear();
    }
    private:
    Deck m_deck;
    ComputerPlayer m_casino;
};

int main()
{
    cout << "\tWelcome to the Blackjack game!"<<endl<<endl;
    BlackJackGame game;

    //the main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while(playAgain)
    {
        game.play();

        //Check whether the player would like to play another round
        cout<<"Would you like another round? (y/n): ";
        cin>>answer;
        cout<<endl;
        playAgain = (answer == 'y' ? true : false);
    }
    cout<<"Game over!";
    return 0;
}

