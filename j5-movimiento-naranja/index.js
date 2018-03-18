const five = require("johnny-five");
const board = new five.Board();

const avgLightBuckets = 32;
let currentLightBucket = 0;

const lightLevels = new Array(avgLightBuckets);
lightLevels.fill(0);

const songChords = {
  0: [
    ['E3', 1 / 4 ], // MI
    ['G3', 1 / 8 ], // SOL
    ['B3', 1 / 4 ], // SI
    ['G3', 1 / 8 ], // SOL
    ['E3', 1 / 2 ], // MI

    ['C4', 1 / 4 ], // DO
    ['B3', 1 / 4 ], // SI
    ['C4', 1 / 4 ], // DO
    [null, 1 ],
  ],
  1: [
    ['G3', 1 / 4 ], // SOL
    ['B3', 1 / 8 ], // SI
    ['D4', 1 / 4 ], // RE
    ['B3', 1 / 8 ], // SI
    ['G3', 1 / 2 ], // SOL

    ['A3', 1 / 4 ], // LA
    ['G3', 1 / 4 ], // SOL
    ['A3', 1 / 4 ], // LA

    [null, 1]
  ]
};

let songSegment = 1;

board.on("ready", function() {
  const photoresistor = new five.Sensor({
    pin: 'A0',
    freq: 100
  });

  const piezo = new five.Piezo(3);

  const updateLight = (lightLevel) => {
    lightLevels[currentLightBucket] = lightLevel;
    currentLightBucket = (currentLightBucket + 1) % avgLightBuckets;

    avgLightLevel = lightLevels.reduce((acc, a) => acc + a, 0) / avgLightBuckets;

    const deltaLightLevel = lightLevel - avgLightLevel;
    const percentageDrop = Math.floor(deltaLightLevel / avgLightLevel * 100);
    if (percentageDrop < -15 && !piezo.isPlaying) {
      songSegment = songSegment ? 0 : 1;
      piezo.play({
        song: songChords[songSegment],
        tempo: 60
      });
    }
  };

  photoresistor.on('data', function onData() {
    updateLight(this.value);
  });
});
