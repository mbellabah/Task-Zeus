# Point of this is to compare to the c++ code

import random, time

start_time = time.time()

class Buyer:
    def __init__(self, addr, quantity):
        self.address = addr
        self.demand = quantity
class Seller:
    def __init__(self, addr, quantity):
        self.address = addr
        self.supply = quantity


buyer_dict = {}
seller_dict = {}
matches_dict = {}
market_size = 1000

def populateEntities():
    for x in range(1, market_size):
        buyer_dict[x] = Buyer(x, random.randint(1,100))
        seller_dict[x] = Seller(x, random.randint(1,100))


def makeMatches():
    global matches_dict
    matches_dict.clear()
    for buyer_element in buyer_dict:
        for seller_element in seller_dict:
            b_name = int(buyer_dict[buyer_element].address)
            s_name = int(seller_dict[seller_element].address)

            base_pair = ("B{},S{}").format(b_name, s_name)
            if float(seller_dict[seller_element].supply) > 0:
                quantity_dev = abs(float(buyer_dict[buyer_element].demand) - float(seller_dict[seller_element].supply))
                matches_dict[base_pair] = quantity_dev

def findBest(buyer):
    quantity_list = []
    smallest_q = 0
    small_dict = {}


    if len(matches_dict) > 0:
        for item in matches_dict:
            if buyer == int(item[1:2]):
                quantity_list.append(matches_dict[item])

    if len(quantity_list) > 0:
        smallest_q = min(quantity_list, key=abs)

    match_seller = 0
    for item in matches_dict:
        name = item.split(',')
        buyer_name = name[0]
        seller_name = name[1]

        if (buyer == int(buyer_name[1:]) and matches_dict[item] == smallest_q):
            match_seller = int(seller_name[1:])
            break

    return match_seller

populateEntities()
makeMatches()
for buyer in buyer_dict:
    print "B: {} => S: {}".format(buyer, findBest(buyer))
print "Done in {} seconds".format(time.time() - start_time)











