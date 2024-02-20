from flask import Flask, request, jsonify
import datetime
import requests
import logging
from model import Lift

import matplotlib
import matplotlib.pyplot as plt
matplotlib.use('Agg')

db = {"liftOccurences": []}

app = Flask(__name__)


def generic_post_route_input_verification(in_dict, expected_keys,
                                          expected_types):
    """
    Generic function for validating an input dictionary

    This function receives the input data to a POST route and a list/tuple of
    expected keys and expected value types.  This function verifies that the
    input data is a dictionary, that it contains the keys founds in the
    expected keys, and that the values for the keys are of the expected type.
    For values that can be strings or int val types, the function checks to see
    if it can be converted to a integer, if not, an error is returned.
    If all verifications pass, a boolean value of True is returned.  If any
    verification fails, a string message explaining the failure is returned.
    Note that the list of expected value types should be in the same order as
    the expected keys.

    Args:
        in_dict (dict/any): the input sent to the POST request.  Ideally, it
                             is a dictionary, but could be any type
        expected_keys (list/tuple): a list or tuple of what keys need to be in
                                    the input dictionary
        expected_types (list/tuple): a tuple of lists of expected value types,
                                     in the same order as the expected keys

    Returns:
        bool or string:  a boolean value of True if all validations pass, a
                         string with a message if a validation fails.
    """

    if type(in_dict) is not dict:
        return "Data sent with post request must be a dictionary."
    for key in expected_keys:
        if key not in in_dict.keys():
            return "{} key is not found in the input".format(key)
    for ex_type, ex_key in zip(expected_types, expected_keys):
        if type(in_dict[ex_key]) not in ex_type:

            if len(ex_type) == 2:

                return "{} key should be of type {}".format(ex_key,
                                                            "string or int")

            else:
                return f"{ex_key} key should be of type {ex_type[0].__name__}"

        if len(ex_type) == 2:

            try:
                int(in_dict[ex_key])
            except ValueError:
                return f"{ex_key} key cannot be converted into type integer"

    return True


@app.route("/post_lift", methods=["POST"])
def post_lift_readings():

    in_dict = request.get_json()


    answer, status = post_lift_driver(in_dict)

    return answer, status


def post_lift_driver(in_dict):
    
    expected_keys = ("count", "avgDuration", "avgHeight")
    expected_types = ([float, int], [float, int], [float, int])
    result = generic_post_route_input_verification(in_dict,
                                                   expected_keys,
                                                   expected_types)
    if result is not True:
        return result, 400
    
    lift_reading = Lift(in_dict["count"],
                        in_dict["avgDuration"],
                        in_dict["avgHeight"])
    
    db["liftOccurences"].append(lift_reading)

    return "Lift Recording Added", 200


@app.route("/lift/<metricType>/", methods=["GET"])
def get_lift_metrics(metricType):

    results, status_code = get_lift_metrics_driver(metricType)
    
    return jsonify(results), status_code


def get_lift_metrics_driver(metricType, time=False):

    if metricType not in ["liftCount", "avgDuration", "avgLiftHeight"]:

        return "Not a Valid Lift Metric", 400
   
    if time:

        pass
    
    else:

        tArr = []
        data = []

        for lift in db["liftOccurences"]:

            tArr.append(lift.timeDate)

            if metricType == "liftCount":
                data.append(lift.liftCount)
            elif metricType == "avgDuration":
                data.append(lift.avgDuration)
            elif metricType == "avgLiftHeight":
                data.append(lift.avgLiftHeight)

        plt.scatter(tArr, data)
        plt.xlabel("Date & Time")
        plt.xlabel(metricType)
        plt.savefig("test.png")


    return "Successfully Obtained Metrics", 200

if __name__ == "__main__":

    app.run(host="0.0.0.0")