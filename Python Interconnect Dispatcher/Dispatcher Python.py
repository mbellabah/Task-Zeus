import serial, random

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
market_size = 4

def instantiateEntities(serial_obj):
    data = serial_obj.readline()
    try:
        data_addr = data[1:2]
        identifier = data[:1]
        quantity = data[3:]
    except:
        return 0

    if identifier == "B":
        buyer_dict[data_addr] = Buyer(data_addr, random.randint(1, 105))
    elif identifier == "S":
        seller_dict[data_addr] = Seller(data_addr, quantity)
    else:
        pass

def build_market(): # Populates the buyer and seller dicts
    global market_size
    count = 0
    while count < market_size:
        instantiateEntities(Dispatcher_S)
        count += 1

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

    small_dict[buyer] = match_seller
    return small_dict

def refresh():
    build_market()
    makeMatches()

def dispatchMatch():
    global seller_dict
    buyer_req = Dispatcher_S.readline()
    if buyer_req[:1] == '!':
        buyer_in_q = int(buyer_req[1])
        my_dict = findBest(buyer_in_q)
        print "MATCH {}".format(my_dict)
        Dispatcher_S.write(str(my_dict[buyer_in_q]))

dispatcher_com = "com5"
Dispatcher_S = serial.Serial(dispatcher_com, 115200, timeout=None)


while True:
    refresh()
    dispatchMatch()











