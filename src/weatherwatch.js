var refreshInterval = 1000 * 60 * 5;   // Refresh info every 5 minutes

var weatherFreq = 0;  // Always pick up the latest weather
var locationFreq = 11; // Update location very hour

var locationCount = 0;  // Number of refreshes until we refresh the location
var weatherCount = 0;  // Number of refreshes until we refresh the weather

var locationOptions = { "timeout": 15000, "maximumAge": 60000 }; 
var latitude =  0;
var longitude = 0;

function fetchWeather() {
  var req = new XMLHttpRequest();
  console.log('Fetch weather');
  req.open('GET', "https://api.forecast.io/forecast/ddb8191c20d47e3cd47c91912e5c200c/"+latitude+","+longitude+"?units=si", true);
  req.onload = function(e) {
    console.log('onload');

    if (req.readyState == 4) {
        console.log('req.readyState == 4');

      if(req.status == 200) {
        console.log(req.responseText);

          var response = JSON.parse(req.responseText);
          var temperature = Math.round(response.currently.temperature);
          var now_forecast = response.currently.summary;
          var hour_forecast = response.minutely.summary;
          var day_forecast = response.hourly.summary;
          var week_forecast = response.daily.summary;

          console.log(temperature);
          console.log(now_forecast);
          console.log(hour_forecast);
          console.log(day_forecast);
          console.log(week_forecast);
          weatherCount = weatherFreq;
          Pebble.sendAppMessage({
            "SUMMARY_NOW":now_forecast,
            "SUMMARY_HOUR":hour_forecast,
            "SUMMARY_DAY":day_forecast,
            "SUMMARY_WEEK":week_forecast
            }
          );
  
      } else {
        console.log("Error");
      }
    }
  }
  req.send(null);
}

function locationSuccess(pos) {
  console.log('Got location');
  latitude = pos.coords.latitude;
  longitude = pos.coords.longitude;
  locationCount = locationFreq;
  fetchWeather();
}

function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  fetchWeather();
}

var emit = function() {
  
  console.log('Emit - but what?');
  if (locationCount == 0) {
    navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
  }
  else {
    locationCount -= 1;
    if (weatherCount == 0) {
      fetchWeather();
    }
    else {
      weatherCount -= 1;
    }
  }
};

Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');

  // Send periodic updates every 3 seconds
  setInterval(emit, refreshInterval);
  
  // And call it for the first time now!
  emit();
});

Pebble.addEventListener('appmessage', function(e) {
  console.log('AppMessage received!');
});

