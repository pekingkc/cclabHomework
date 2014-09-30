var city = "";
var state = "";
var APIKey = "ce7aae6fa27e2dc0";

//loadWeather
var loadWeather = function(response){

	if(response.response.error){
		alert(response.response.error.description);
		return;
	}

	//get the city 
	var thisCity = response.current_observation.display_location.city;
	var temp = response.current_observation.temp_c;
	var weather = response.current_observation.weather;

	//show the value in the web. And change the temperature number into round number
	$("#temperature").text(temp.toFixed(0));
	$("#condition").text(weather);

	//change the canvas's background color deponds on temperature.
	if(temp < 20 && temp > 15){
		$("#canvas").css("background-color","#78f707");
		$("#condition").css("color","#78f707");
	}else if(temp <15){
		$("#canvas").css("background-color","#06fcf3");
		$("#condition").css("color","#06fcf3");
	}
	else{ //change the color back to yellow
		$("#canvas").css("background-color","#fff000"); 
		$("#condition").css("color","#fff000");
	}
	
	
}



//get weather funciton
var getWeather = function(){

	var thisURL = "http://api.wunderground.com/api/" + APIKey + "/conditions/q/" + state + "/" + city + ".json";

	console.log(thisURL);
	$.ajax({
		url : thisURL,
		dataType : "jsonp",
		success : function(response){
			loadWeather(response);
			console.log(response);
		}
	});

}

//set location function
var setLocation = function(){

	//get the value of "city" and "state"
	city = $("#currentCity").val();
	state = $("#currentState").val();
	
	console.log("you want the weather for" + city + "," + state);
	getWeather();

	//show the place beside the input div
	$("#place").text("@" + city);

	if(city == null || city == ""){

		alert("Man! You need to list a city with correct name!");
		return;

	}

	//clear the input div after "go"
	city = $("#currentCity").val("");

}


//initial the brooklyn weather when first run
var initWeather = function(){
		var thisURL = "http://api.wunderground.com/api/" + APIKey + "/conditions/q/NY/brooklyn.json";

		console.log(thisURL);
		$.ajax({
			url : thisURL,
			dataType : "jsonp",
			success : function(response){
				loadWeather(response);
				console.log(response);
			}
		});

}


//first run
var init = function(){

	console.log("what\'s the weather");
	initWeather();
	$("#place").text("@brooklyn");

	//press btn to go through the procedure
	$("#submit").click(function(e){
		e.preventDefault();
		setLocation();
	});

}

//every begining, run init()
$(document).ready(function(){

	init();

});