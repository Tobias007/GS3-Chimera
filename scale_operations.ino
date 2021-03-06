void manageScaleConnection()
{
#ifdef ACAIA_LUNAR_INTEGRATION
	if (!scaleConnected)
	{
		if (millis() > scaleReconnectionTimer + 1000) 
		{	
			//connectScale();
			scaleConnected = scale->isScaleConnected(); 
			scaleReconnectionTimer = millis();
		}
	}
	else 
	{
	scaleConnected = scale->isScaleConnected();
	scaleReconnectionTimer = millis();
	}	
#endif	
}

	
/*
Autopower off
else if(scaleActive && millis() > 10000 + scaleIdleTimer ) 
{	
	scale->disconnect();
}
*/

//check if there is a Portafilter to do a custom dose.
void updateDoseWeight()
{	
#ifdef ACAIA_LUNAR_INTEGRATION		
	if(scaleConnected)
		{
		updateWeight();
		int doseStabilityCounter = 8;
		float previousWeight = scaleWeight;
		float epsilon = 0.05;
		while (previousWeight > EMPTY_PORTAFILTER_WEIGHT)
		{
			//scaleWeight = measureWeight(previousWeight);
			if (updateWeight()) 
			{
				displayPressureandWeight();
				if (abs(scaleWeight - previousWeight) < epsilon)
					doseStabilityCounter--;
				else
					doseStabilityCounter=8;
			}
			previousWeight = scaleWeight;
			if (doseStabilityCounter == 0)
			{
				currentDose = scaleWeight - EMPTY_PORTAFILTER_WEIGHT;
				break;
			}
		}
	}
#endif
}
		
boolean updateWeight()
{  
#ifdef ACAIA_LUNAR_INTEGRATION
	if (scaleConnected)
	{
		manageScaleConnection();
		scale->update();
		if (scale->hasWeightChanged()) 
		{
			float a = scale->getWeight(); // get new weight and convert to the nearest 1/10
			int b = a * 10;
			scaleWeight = float(b / 10.0); 
			scaleBattery = scale->getBattery();
			return true;
		}
	}
#endif
	return false; 
}

void disconnectScale()
{
#ifdef ACAIA_LUNAR_INTEGRATION
	scale->disconnect();
	scaleConnected = scale->isScaleConnected();
#endif
}

void connectScale()
{
#ifdef ACAIA_LUNAR_INTEGRATION
	scale = new Scale();
	scale->connect();
	scaleConnected = scale->isScaleConnected();
	scaleReconnectionTimer = millis();
#endif
}

#ifdef ACAIA_LUNAR_INTEGRATION
//Display Battery
void displayBattery()
{
	if (scaleConnected)
	{
		tft.drawRect(230, 105, 4, 5, ILI9341_LIGHTGREY); //+210
		tft.drawRect(213, 103, 18, 9, ILI9341_LIGHTGREY);
		long h = 16 * scaleBattery / 100;
		
		if (scaleBattery > 40)
		{
			tft.drawPixel(216,112,bg_Color);
			tft.drawPixel(227,102,bg_Color);
			tft.fillRect(214+h, 104, 16-h, 7, bg_Color);
			tft.fillRect(214, 104, h, 7, ILI9341_GREEN); 
		}
		else if (scaleBattery > 20)
		{
			tft.drawPixel(216,112,bg_Color);
			tft.drawPixel(227,102,bg_Color);
			tft.fillRect(214+h, 104, 16-h, 7, bg_Color);			
			tft.fillRect(214, 104, h, 7, ILI9341_YELLOW); 
		}
		else if (scaleBattery > 14)
		{
			tft.drawPixel(216,112,bg_Color);
			tft.drawPixel(227,102,bg_Color);
			tft.fillRect(217, 104, 13, 7, bg_Color);			
			tft.fillRect(214, 104, 3, 7, ILI9341_RED);
		}
		else if (scaleBattery > 8)
		{
			tft.drawPixel(216,112,bg_Color);
			tft.drawPixel(227,102,bg_Color);
			tft.fillRect(216, 104, 14, 7, bg_Color);			
			tft.fillRect(214, 104, 2, 7, ILI9341_RED);
		}
		else
		{
			tft.fillRect(216, 104, 14, 7, bg_Color);			
			tft.fillRect(214, 104, 2, 7, ILI9341_MAROON);
			tft.drawLine(216,112,227,102, text_light_Color);
		}
	}
	else
	{
		tft.drawPixel(216,112,bg_Color);
		tft.drawPixel(227,102,bg_Color);
		tft.fillRect(231, 105, 5, 5, bg_Color);
		tft.fillRect(213, 103, 18, 9, bg_Color);
		
	}
}
#endif