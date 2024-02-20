import datetime


class Lift:
    def __init__(self, liftCount = 0, avgDuration = 0, avgLiftHeight = 0):


        self.timeDate = datetime.datetime.now()
        self.liftCount = liftCount
        self.avgDuration = avgDuration
        self.avgLiftHeight = avgLiftHeight
    
    