// tests go here; this will not be compiled when this package is used as an extension.
virtualride.advertise()
virtualride.setResistanceLevel(1)
basic.forever(function () {
    basic.pause(1000)
    virtualride.calcAndNotifyIndoorBikeData(975609)
})
virtualride.onFitnessMachineControlPoint(virtualrideFMCP.SET_TARGET_RESISTANCE_LEVEL, function () {
    basic.showNumber(virtualride.getTargetResistanceLevel10())
})
virtualride.onFitnessMachineControlPoint(virtualrideFMCP.SET_INDOOR_BIKE_SIMULATION, function () {
    basic.showNumber(virtualride.getGrade100())
})
