#include "Orders.h"

void testOrdersList() {
	OrdersList oList{};
	
	oList.addOrder(new Deploy{50, "China"});
	oList.addOrder(new Advance{50, "China", "Japan"});
	oList.addOrder(new Bomb{"Japan"});
	oList.addOrder(new Blockade{"Japan"});
	oList.addOrder(new Airlift{50, "China", "Japan"});
	oList.addOrder(new Negotiate{"Joseph"});
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "\n***Orders***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		//o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.remove(0);

	std::cout << "\n***Orders after remove***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		//o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.move(0, 3);
	std::cout << "\n***Orders after move***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		//o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "\n***OrdersList***" << std::endl << std::endl;
	std::cout << oList << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}