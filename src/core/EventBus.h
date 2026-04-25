#pragma once
                // think of it like a post office
enum EventType  //this is just a list of all possible types of letters that can be sent
{
	enemieskilled,
	playerhit,
	levelcomplete,
	powerupcollected,
};

struct Event
{
	EventType type; //what kind of letter is this
	int data; //any extra info inside the letter
};

class EventBus //post office 
{
private:
	Event arr[50]; // this will store 50 event objects // holds a mailbox of 50 letters
	int queuedevents; // tracks how many letters are currently waiting 

public:

	EventBus()
	{
		queuedevents = 0;
	}
	
	void post(EventType type, int data = 0)
	{
		if (queuedevents < 50)
		{
			arr[queuedevents].type = type;
			arr[queuedevents].data = data;
			queuedevents++;
		}
	}

	int hasEvent(EventType type)
	{
		for(int i=0;i<queuedevents;i++)
		{
			if (arr[i].type == type)
				return arr[i].data;
				//return true;
		}
		return -1;
	//return false;
	}

	void clear()
	{
		queuedevents = 0;
	}

};

// flow:
// Enemy dies ? someone drops a letter saying "ENEMY_KILLED" into the post office
//GemSystem checks the post office ? finds that letter ? gives gems
//End of frame ? post office empties out ? ready for tomorrow