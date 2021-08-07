function OrbiterClient() { 
}

OrbiterClient.prototype.cgi = "/cgi";
OrbiterClient.prototype.subs = new Array();

OrbiterClient.prototype.update = function() {
	if ( this.subs.length == 0 )
		return ;

	var query = "";
	for (var i=0; i<this.subs.length; i++) {
		query = query + this.subs[i].query + "|";
	}
	
	var me = this;
	this.execute(query, function(data) {
		var resp = data.split("|");
		for (var i=0; i<me.subs.length; i++) {
			me.subs[i].callback(resp[i]);
		}
	});
}

OrbiterClient.prototype.start = function(interval) {
	// http://stackoverflow.com/questions/18263585/call-javascript-object-method-with-setinterval
	var me = this;
	this.interval = setInterval(function () {
		me.update();
	}, interval);
}

OrbiterClient.prototype.execute = function(query, callback) {
	$.post(this.cgi, { cmd : query }, function(data) {
		callback( data.slice(0, data.length-1));
	});
}

OrbiterClient.prototype.subscribe = function(query, callback) {
	this.subs.push( { query : query, callback : callback } );
}

// example: oc.sendBufferedKey(OAPI_KEY_A, OAPI_KEY_LCONTROL);
OrbiterClient.prototype.sendBufferedKey = function(key, modifier1, modifier2, modifier3, callback) {
	var query = "SHIP:FOCUS:SendBufferedKey:" + key;
	if ( modifier1 != null ) 
		query = query + ":" + modifier1;
	
	if ( modifier2 != null ) 
		query = query + ":" + modifier2;
	
	if ( modifier3 != null ) 
		query = query + ":" + modifier3;
	
	this.execute(query, callback);
}

OrbiterClient.prototype.fireThrusterGroup = function(group, level, time_ms) {
	var query = "SHIP:FOCUS:SetEngineGrpLevel:"+group+":"+level;
	console.log(query);
	var me = this;
	this.execute(query, function(data) { 
		console.log(data);
	
		if ( time_ms != null ) {
			setTimeout( function() {
				var q = "SHIP:FOCUS:SetEngineGrpLevel:"+group+":0";
				console.log(q);
				me.execute(q, function(d) { console.log(d) } );		
			}, time_ms );
		}
	
	} );
}

// Key codes copied from OrbiterAPI.h
var OAPI_KEY_ESCAPE			= 0x01  ; // Escape key
var OAPI_KEY_1				= 0x02  ; // '1' key on main keyboard
var OAPI_KEY_2				= 0x03  ; // '2' key on main keyboard
var OAPI_KEY_3				= 0x04  ; // '3' key on main keyboard
var OAPI_KEY_4				= 0x05  ; // '4' key on main keyboard
var OAPI_KEY_5				= 0x06  ; // '5' key on main keyboard
var OAPI_KEY_6				= 0x07  ; // '6' key on main keyboard
var OAPI_KEY_7				= 0x08  ; // '7' key on main keyboard
var OAPI_KEY_8				= 0x09  ; // '8' key on main keyboard
var OAPI_KEY_9				= 0x0A  ; // '9' key on main keyboard
var OAPI_KEY_0				= 0x0B  ; // '0' key on main keyboard
var OAPI_KEY_MINUS			= 0x0C  ; // '-' key on main keyboard
var OAPI_KEY_EQUALS			= 0x0D  ; // '=' key on main keyboard
var OAPI_KEY_BACK			= 0x0E  ; // backspace key
var OAPI_KEY_TAB			= 0x0F  ; // tab key
var OAPI_KEY_Q				= 0x10  ; //< 'Q' key
var OAPI_KEY_W				= 0x11  ; //< 'W' key
var OAPI_KEY_E				= 0x12  ; //< 'E' key
var OAPI_KEY_R				= 0x13  ; //< 'R' key
var OAPI_KEY_T				= 0x14  ; //< 'T' key
var OAPI_KEY_Y				= 0x15  ; //< 'Y' key
var OAPI_KEY_U				= 0x16  ; // 'U' key
var OAPI_KEY_I				= 0x17  ; // 'I' key
var OAPI_KEY_O				= 0x18  ; // 'O' key
var OAPI_KEY_P				= 0x19  ; // 'P' key
var OAPI_KEY_LBRACKET		= 0x1A  ; // '[' (left bracket) key
var OAPI_KEY_RBRACKET		= 0x1B  ; // ']' (right bracket) key
var OAPI_KEY_RETURN			= 0x1C  ; // 'Enter' key on main keyboard
var OAPI_KEY_LCONTROL		= 0x1D  ; // Left 'Ctrl' key
var OAPI_KEY_A				= 0x1E  ; // 'A' key
var OAPI_KEY_S				= 0x1F  ; // 'S' key
var OAPI_KEY_D				= 0x20  ; // 'D' key
var OAPI_KEY_F				= 0x21  ; // 'F' key
var OAPI_KEY_G				= 0x22  ; // 'G' key
var OAPI_KEY_H				= 0x23  ; // 'H' key
var OAPI_KEY_J				= 0x24  ; // 'J' key
var OAPI_KEY_K				= 0x25  ; // 'K' key
var OAPI_KEY_L				= 0x26  ; // 'L' key
var OAPI_KEY_SEMICOLON		= 0x27  ; // ';' (semicolon) key
var OAPI_KEY_APOSTROPHE		= 0x28  ; // ' (apostrophe) key
var OAPI_KEY_GRAVE			= 0x29  ; // accent grave
var OAPI_KEY_LSHIFT			= 0x2A  ; // Left 'Shift' key
var OAPI_KEY_BACKSLASH		= 0x2B  ; // '\' (Backslash) key
var OAPI_KEY_Z				= 0x2C  ; // 'Z' key
var OAPI_KEY_X				= 0x2D  ; // 'X' key
var OAPI_KEY_C				= 0x2E  ; // 'C' key
var OAPI_KEY_V				= 0x2F  ; // 'V' key
var OAPI_KEY_B				= 0x30  ; // 'B' key
var OAPI_KEY_N				= 0x31  ; // 'N' key
var OAPI_KEY_M				= 0x32  ; // 'M' key
var OAPI_KEY_COMMA			= 0x33  ; // ',' (comma) key
var OAPI_KEY_PERIOD			= 0x34  ; // '.' key on main keyboard
var OAPI_KEY_SLASH			= 0x35  ; // '/' key on main keyboard
var OAPI_KEY_RSHIFT			= 0x36  ; // Right 'Shift' key
var OAPI_KEY_MULTIPLY		= 0x37  ; // * on numeric keypad
var OAPI_KEY_LALT			= 0x38  ; // left Alt
var OAPI_KEY_SPACE			= 0x39  ; // 'Space' key
var OAPI_KEY_CAPITAL		= 0x3A  ; // caps lock key
var OAPI_KEY_F1				= 0x3B  ; // F1 function key
var OAPI_KEY_F2				= 0x3C  ; // F2 function key
var OAPI_KEY_F3				= 0x3D  ; // F3 function key
var OAPI_KEY_F4				= 0x3E  ; // F4 function key
var OAPI_KEY_F5				= 0x3F  ; // F5 function key
var OAPI_KEY_F6				= 0x40  ; // F6 function key
var OAPI_KEY_F7				= 0x41  ; // F7 function key
var OAPI_KEY_F8				= 0x42  ; // F8 function key
var OAPI_KEY_F9				= 0x43  ; // F9 function key
var OAPI_KEY_F10			= 0x44  ; // F10 function key
var OAPI_KEY_NUMLOCK		= 0x45  ; // 'Num Lock' key
var OAPI_KEY_SCROLL			= 0x46  ; // Scroll lock
var OAPI_KEY_NUMPAD7		= 0x47  ; // '7' key on numeric keypad
var OAPI_KEY_NUMPAD8		= 0x48  ; // '8' key on numeric keypad
var OAPI_KEY_NUMPAD9		= 0x49  ; // '9' key on numeric keypad
var OAPI_KEY_SUBTRACT		= 0x4A  ; // '-' key on numeric keypad
var OAPI_KEY_NUMPAD4		= 0x4B  ; // '4' key on numeric keypad
var OAPI_KEY_NUMPAD5		= 0x4C  ; // '5' key on numeric keypad
var OAPI_KEY_NUMPAD6		= 0x4D  ; // '6' key on numeric keypad
var OAPI_KEY_ADD			= 0x4E  ; // '+' key on numeric keypad
var OAPI_KEY_NUMPAD1		= 0x4F  ; // '1' key on numeric keypad
var OAPI_KEY_NUMPAD2		= 0x50  ; // '2' key on numeric keypad
var OAPI_KEY_NUMPAD3		= 0x51  ; // '3' key on numeric keypad
var OAPI_KEY_NUMPAD0		= 0x52  ; // '0' key on numeric keypad
var OAPI_KEY_DECIMAL		= 0x53  ; // '.' key on numeric keypad
var OAPI_KEY_OEM_102		= 0x56  ; // | \< \> on UK/German keyboards
var OAPI_KEY_F11			= 0x57  ; // F11 function key
var OAPI_KEY_F12			= 0x58  ; // F12 function key
var OAPI_KEY_NUMPADENTER	= 0x9C  ; // Enter on numeric keypad
var OAPI_KEY_RCONTROL		= 0x9D  ; // right Control key
var OAPI_KEY_DIVIDE			= 0xB5  ; // '/' key on numeric keypad
var OAPI_KEY_RALT           = 0xB8  ; // right Alt
var OAPI_KEY_HOME           = 0xC7  ; // Home on cursor keypad
var OAPI_KEY_UP             = 0xC8  ; // up-arrow on cursor keypad
var OAPI_KEY_PRIOR          = 0xC9  ; // PgUp on cursor keypad
var OAPI_KEY_LEFT           = 0xCB  ; // left-arrow on cursor keypad
var OAPI_KEY_RIGHT          = 0xCD  ; // right-arrow on cursor keypad
var OAPI_KEY_END            = 0xCF  ; // End on cursor keypad
var OAPI_KEY_DOWN           = 0xD0  ; // down-arrow on cursor keypad
var OAPI_KEY_NEXT           = 0xD1  ; // PgDn on cursor keypad
var OAPI_KEY_INSERT         = 0xD2  ; // Insert on cursor keypad
var OAPI_KEY_DELETE         = 0xD3  ; // Delete on cursor keypad

var THGROUP_MAIN = 0;
var THGROUP_RETRO = 1; 
var THGROUP_HOVER = 2;
var THGROUP_ATT_PITCHUP = 3;
var THGROUP_ATT_PITCHDOWN = 4;
var THGROUP_ATT_YAWLEFT = 5;
var THGROUP_ATT_YAWRIGHT = 6; 
var THGROUP_ATT_BANKLEFT = 7;
var THGROUP_ATT_BANKRIGHT = 8;
var THGROUP_ATT_RIGHT = 9;
var THGROUP_ATT_LEFT = 10;
var THGROUP_ATT_UP = 11;
var THGROUP_ATT_DOWN = 12;
var THGROUP_ATT_FORWARD = 13;
var THGROUP_ATT_BACK = 14; 
var THGROUP_USER = 0x40; 

