import requests
import random


server = "http://127.0.0.1:5000"




for i in range(10):

    out_dict = {"count": random.randint(0, 10),
                "avgDuration": random.randint(0, 20),
                "avgHeight": random.randint(0, 15)}


    r = requests.post(server + "/post_lift", json=out_dict)
    print(r.text)
    print(r.status_code)


r = requests.get(server + "/lift/avgDuration")
print(r.text)
print(r.status_code)