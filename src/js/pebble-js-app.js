var allStops=[];
var currentStop = 0;

function loadLocation(position) {
	console.log(position.coords.latitude*1000000, position.coords.longitude*1000000);
	var req = new XMLHttpRequest();
	req.open('GET', 'http://m.bahn.de/bin/query.exe/dny?performLocating=2&tpl=stop2json&look_maxno=10&look_maxdist=1000&look_stopclass=1023&look_x=' + (position.coords.longitude*1000000) + '&look_y=' + (position.coords.latitude*1000000), true);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if(req.status == 200) {
				var response = JSON.parse(req.responseText);
				if (typeof response['stops'][0]['name'] !== "undefined") {
					allStops = response['stops'];
					currentStop = 0;
					showStop()
				}
			} else { console.log("Error"); }
		}
	}
	req.send(null);
}

function showStop() {
	if (typeof allStops[currentStop] !== "undefined") {
		Pebble.sendAppMessage({
			"0":allStops[currentStop]['name'].replace(/,/,",\n")+"\n"+allStops[currentStop]['dist']+"m"
		}, function(){}, function(){});
	} else if (currentStop === 0) {
		Pebble.sendAppMessage({
			"0":"Haltestelle suchen..."
		}, function(){}, function(){});
	} else {
		currentStop = 0;
		showStop();
	}
}

Pebble.addEventListener("appmessage",
  function(e) {
    currentStop += e.payload["1"];
    showStop();
  }
);


Pebble.addEventListener("ready",
    function(e) {
    	navigator.geolocation.getCurrentPosition(loadLocation);
        console.log("Hello world! - Sent from your javascript application.");
    }
);
