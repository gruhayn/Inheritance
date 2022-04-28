#include <iostream>
#include <string>
#include <conio.h>
#include <random>

class Dice
{
public:
	int Roll( int nDice )
	{
		int total = 0;
		for( int n = 0; n < nDice; n++ )
		{
			total += d6( rng );
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>( 1,6 );
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};

class MemeFighter
{

public:
	
	std::string GetName() const
	{
		return name;
	}

	bool IsAlive() const {
		return hp > 0;
	}

	int GetInitiative() {
		return speed + d.Roll(2);
	}

	void Punch(MemeFighter& other) const {
		if (IsAlive()) {
			other.hp -= power + d.Roll(2);
		}
	}

	virtual void Tick() {
		if (IsAlive()) {
			hp += d.Roll(6);
		}
	}

	virtual void SpecialMove(MemeFighter&) {}

protected:
	MemeFighter(std::string name, int hp, int speed, int power)
		:
		name(name),
		hp(hp),
		speed(speed),
		power(power)
	{}

	int Roll(int nDice = 1) {
		return d.Roll(nDice);
	}

	void SetDamage(MemeFighter other, int damage) const {
		other.hp -= damage;
	}

	

protected:
	std::string name;
	int hp;
	int speed;
	int power;

private:
	mutable Dice d;

};

class MemeFrog : public MemeFighter
{
public:

	MemeFrog(std::string name)
		:
		MemeFighter(name, 69, 7, 14)
	{}

	void SpecialMove(MemeFighter& other) override
	{
		if (Roll() > 4) 
		{
			SetDamage(other, Roll(3) + 20);
		}

	}

	void Tick() override 
	{
		if (IsAlive()) {
			SetDamage(*this, Roll());
			MemeFighter::Tick();
		}
	}

};


class MemeStoner : public MemeFighter
{
public:
	MemeStoner(std::string name)
		:
		MemeFighter(name, 80, 4, 10)
	{}

	void SpecialMove(MemeFighter&) override
	{
		if (Roll() > 3)
		{
			speed += 3;
			power *= 69 / 42;
			hp = hp + 10;
		}

	}

};


void Engage( MemeFighter& f1,MemeFighter& f2 )
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if( p1->GetInitiative() < p2->GetInitiative() )
	{
		std::swap( p1,p2 );
	}
	// execute attacks
	p1->Punch( *p2 );
	p2->Punch( *p1 );
}

void DoSpecials(MemeFighter & m1, MemeFighter& m2) 
{
	auto* f1 = &m1;
	auto* f2 = &m2;

	if (f1->GetInitiative() < f2->GetInitiative()) 
	{
		std::swap(f1, f2);
	}

	// execute attacks
	f1->SpecialMove(*f2);
	f2->SpecialMove(*f1);

}

int main()
{
	MemeFrog f1( "Dat Boi" );
	MemeStoner f2( "Good Guy Greg" );

	while( f1.IsAlive() && f2.IsAlive() )
	{
		// trade blows
		Engage( f1,f2 );
		// special moves
		DoSpecials(f1, f2);
		// end of turn maintainence
		f1.Tick();
		f2.Tick();

		std::cout << "Press any key to continue...";
		while( !_kbhit() );
		_getch();
		std::cout << std::endl << std::endl;
	}

	if( f1.IsAlive() )
	{
		std::cout << f1.GetName() << " is victorious!";
	}
	else
	{
		std::cout << f2.GetName() << " is victorious!";
	}
	while( !_kbhit() );
	return 0;
}