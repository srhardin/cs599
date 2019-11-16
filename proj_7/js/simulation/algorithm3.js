//algorithm 03function algorithm3(){	var start_day = date_hash[g_StartDate];	var end_day = date_hash[g_EndDate];	var num_days = end_day - start_day + 1;	max_drawdown = 0;	prev_maxima = +g_StartMoney;	var balance = +g_StartMoney;	value_over_time = [g_StartMoney];	gains_losses = [];	var owned_stocks = [];	for(day = start_day; day <= end_day; ++day)	{		var sell_stocks = [];		var buy_stocks = [];		//randomly sell stocks with exponential distribution		var revenue = 0;		for(i = owned_stocks.length - 1; i >= 0; --i)		{			var roll = Math.random();			var prob = 0.5 ** owned_stocks[i].days_owned;			if(roll > prob)			{				revenue += data[owned_stocks[i].id][day].Adjusted_close * owned_stocks[i].num_owned;				owned_stocks[i] = owned_stocks[owned_stocks.length - 1];				owned_stocks.pop();			}		}		//buy stocks at random until no money left		var new_balance = revenue + balance;		while(true)		{			var index = Math.floor(Math.random() * num_stocks);			var price = +data[index][day].Adjusted_close;			if(price > new_balance)			{				break;			}			var num_to_buy = 1;			let base_stock =			{				id : index,				price : +data[index][day].Adjusted_close,				diff : +data[index][day].Adjusted_close - +data[index][day - 1].Adjusted_close,				days_owned : 0,				num_owned : num_to_buy			};			owned_stocks.push(base_stock);			new_balance -= price * num_to_buy;		}		//calculate new account value		var value = 0;		for(i = 0; i < owned_stocks.length; ++i)		{			var id = owned_stocks[i].id;			var num_owned = owned_stocks[i].num_owned;			value += +data[id][day].Adjusted_close * num_owned;		}		//calculate percent gain/loss		var net_worth = value + new_balance;		gains_losses.push((1.0 * net_worth) / value_over_time[value_over_time.length - 1] - 1);		value_over_time.push(net_worth);						//calculate drawdown		var drawdown = (prev_maxima - net_worth) / prev_maxima;		if(drawdown < 0)		{			prev_maxima = net_worth;		}		else if(drawdown > max_drawdown)		{			max_drawdown = drawdown;		}		//update number of days stocks have been owned		for(i = 0; i < owned_stocks.length; ++i)		{			++owned_stocks[i].days_owned;		}		//update current balance		balance = new_balance;		    	//update stock ownerships    	var owned_array = new Array(num_stocks).fill(0);    	for(i = 0; i < owned_stocks.length; ++i)    	{    	  owned_array[owned_stocks[i].id] += owned_stocks[i].num_owned;    	}    	g_OwnedPerDay.push(owned_array);	} //end day loop} //end algorithm3()