#include "Orders.h"

void testOrdersList() {
	OrdersList oList{};
	
	oList.addOrder(new Deploy);
	oList.addOrder(new Advance);
	oList.addOrder(new Bomb);
	oList.addOrder(new Blockade);
	oList.addOrder(new Airlift);
	oList.addOrder(new Negotiate);
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "\n***Orders***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.remove(0);

	std::cout << "\n***Orders after remove***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.move(0, 3);
	std::cout << "\n***Orders after move***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "\n***OrdersList***" << std::endl << std::endl;
	std::cout << oList << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}