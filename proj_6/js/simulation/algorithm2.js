//algorithm 02function algorithm2(){	var start_day = date_hash[g_StartDate];	var end_day = date_hash[g_EndDate];	var num_days = end_day - start_day + 1;	max_drawdown = 0;	prev_maxima = g_StartMoney;	var balance = g_StartMoney;	value_over_time = [g_StartMoney];	gains_losses = [];	var owned_stocks = [];	for(day = start_day; day <= end_day; ++day)	{		if(day == 0) continue;		var sell_stocks = [];		var buy_stocks = [];					//identify stocks to buy/sell		var differences = [];		for(i = 0; i < num_stocks; ++i)		{			let base_stock =			{				id : i,				price : +data[i][day].Adjusted_close,				diff : +data[i][day].Adjusted_close - +data[i][day - 1].Adjusted_close,				days_owned : 0,				num_owned : 0			};			differences.push(base_stock);		}		//sort moved to after identifying sell stocks		//identify owned stocks to sell		for(i = 0; i < owned_stocks.length; ++i)		{			//want to sell if price went up and profit will be made			//var curr_diff = differences[owned_stocks[i].id].diff;			var profit = differences[owned_stocks[i].id].price - owned_stocks[i].price;			var days_owned = owned_stocks[i].days_owned;			if((profit > 0 || days_owned > 20) &&			   data[owned_stocks[i].id][day].Volume > 10000)			{				sell_stocks.push(owned_stocks[i]);			}		}		differences.sort(function(a, b){return a.diff-b.diff;});		//want to buy the stocks that dropped the most (back)		//value, not percent		for(i = 0; differences[i].diff < -0.1 && i < num_stocks; ++i)		{			if(data[differences[i].id][day].Volume > 10000)			{				buy_stocks.push(differences[i]);			}		}		//sell stocks		var revenue = 0;		for(i = owned_stocks.length - 1; i >= 0; --i)		{			for(j = 0; j < sell_stocks.length; ++j)			{				if(owned_stocks[i].id == sell_stocks[j].id)				{					//BUG - sells all stocks of a company even if only one purchase has been held too long					revenue += data[owned_stocks[i].id][day].Adjusted_close * owned_stocks[i].num_owned;					owned_stocks[i] = owned_stocks[owned_stocks.length - 1];					owned_stocks.pop();					break;				}			}		}		//buy stocks		//NOTE - add outer loop?		var new_balance = revenue + balance;		for(i = 0; i < buy_stocks.length; ++i)		{			//use difference to control number to buy			var desired_num = Math.floor(buy_stocks[i].diff * -10);			while(desired_num > 0)			{				if(buy_stocks[i].price * desired_num <= new_balance)				{					buy_stocks[i].num_owned = desired_num;					owned_stocks.push(buy_stocks[i]);					new_balance -= buy_stocks[i].price * desired_num;					break;				}				--desired_num;			}		}		//calculate new account value		var value = 0;		for(i = 0; i < owned_stocks.length; ++i)		{			var id = owned_stocks[i].id;			var num_owned = owned_stocks[i].num_owned;			value += data[id][day].Adjusted_close * num_owned;		}		//calculate percent gain/loss		var net_worth = value + new_balance;		gains_losses.push((1.0 * net_worth) / value_over_time[value_over_time.length - 1] - 1);		value_over_time.push(net_worth);						//calculate drawdown		var drawdown = (prev_maxima - net_worth) / prev_maxima;		if(drawdown < 0)		{			prev_maxima = net_worth;		}		else if(drawdown > max_drawdown)		{			max_drawdown = drawdown;		}		//update number of days stocks have been owned		for(i = 0; i < owned_stocks.length; ++i)		{			++owned_stocks[i].days_owned;		}		//update current balance		balance = new_balance;		    //update stock ownerships    var owned_array = new Array(num_stocks).fill(0);    for(i = 0; i < owned_stocks.length; ++i)    {      owned_array[owned_stocks[i].id] += owned_stocks[i].num_owned;    }    g_OwnedPerDay.push(owned_array);	}	//TODO - check all these are needed and correct	//g_EndMoney = balance;    //BuildGainsHistogram();    //avGraph.update(value_over_time);    //SetAllStats();}