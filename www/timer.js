function Timer() {
}

Timer.prototype.zero = 0;

Timer.prototype.reset = function() {
	this.zero = this.last;
}

Timer.prototype.getTime = function(t) {
	this.last = t;
	t1 = t - this.zero;
	var sec_num = parseInt(t1, 10); // don't forget the second param
	var hours   = Math.floor(sec_num / 3600);
	var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
	var seconds = sec_num - (hours * 3600) - (minutes * 60);

	if (hours   < 10) {hours   = "0"+hours;}
	if (minutes < 10) {minutes = "0"+minutes;}
	if (seconds < 10) {seconds = "0"+seconds;}
	var time    = hours+' '+minutes+' '+seconds;
	return time;
}
