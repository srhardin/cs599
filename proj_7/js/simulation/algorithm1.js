//algorithm 01
function algorithm1()
{
  var start_day = date_hash[g_StartDate];
  var end_day = date_hash[g_EndDate];
  var num_days = end_day - start_day + 1;

  max_drawdown = 0;
  prev_maxima = g_StartMoney;
  var balance = g_StartMoney;

  value_over_time = [g_StartMoney];
  gains_losses = [];
  var owned_stocks = [];
  g_OwnedPerDay = [];

  for(day = start_day; day <= end_day; ++day)
  {
    var sell_stocks = [];
    var buy_stocks = [];

    //cumulative mean for each stock
    var cumulative_means = [];
    var cumulative_devs = [];
    var g_MaxMavgDays = 20;
    for(i = 0; i < num_stocks; ++i)
    {
      //calculate mavg for each number of days
      var moving_avg = 0;
      var invalid_flag = false;
      for(j = 2; j <= g_MaxMavgDays; ++j)
      {
        var mavg = 0;
        for(k = 0; k < j; ++k)
        {
          if(day - k < 0 || +data[i][day - k].Volume == 0)
          {
            invalid_flag = true;
            break;
          }
          mavg += +data[i][day - k].Adjusted_close;
        }
        if(invalid_flag)
        {
          break;
        }
        moving_avg += mavg / j;

      }

      if(invalid_flag)
      {
        cumulative_means.push(0);
      }
      else
      {
        cumulative_means.push(moving_avg / (g_MaxMavgDays - 1.0));
      }
    }
      
    //identify stocks to buy/sell
    var differences = [];
    for(i = 0; i < num_stocks; ++i)
    {
      //NOTE- could use  different method of calculating mean
      var mean = cumulative_means[i];
      var dev;
      if(mean == 0)
      {
        dev = 0;
      }
      else
      {
        dev = +(data[i][day].Adjusted_close - mean) / mean;
      }
      let base_stock =
      {
        id : i,
        price : +data[i][day].Adjusted_close,
        diff : dev,
        days_owned : 0,
        num_owned : 0
      };
      differences.push(base_stock);
    }
    differences.sort(function(a, b){return a.diff-b.diff;});

    //identify owned stocks to sell
    for(i = 0; i < owned_stocks.length; ++i)
    {
      //want to sell once the stock rises above mean
      //also don't want to hold stocks for too long
      var curr_diff = 0;
      for(j = 0; j < num_stocks; ++j)
      {
        if(differences[j].id == owned_stocks[i].id)
        {
           curr_diff = differences[j].diff;
        }
      }
      var purchase_diff = owned_stocks[i].diff;
      var days_owned_weight = owned_stocks[i].days_owned * 0.01;
      var diff = curr_diff - purchase_diff + days_owned_weight;
      if(diff > 0.2)
      {
        sell_stocks.push(owned_stocks[i]);
      }
    }

    //identify stocks that have deviated the most from the mean
    //want to buy the stocks that dropped the most (back)
    for(i = 0; differences[i].diff < -0.08 && i < num_stocks; ++i)
    {
      buy_stocks.push(differences[i]);
    }

    //sell stocks
    var revenue = 0;
    for(i = owned_stocks.length - 1; i >= 0; --i)
    {
      for(j = 0; j < sell_stocks.length; ++j)
      {
        if(owned_stocks[i].id == sell_stocks[j].id)
        {
          //BUG - sells all stocks of a company even if only one purchase has been held too long
          revenue += data[owned_stocks[i].id][day].Adjusted_close * owned_stocks[i].num_owned;
          owned_stocks[i] = owned_stocks[owned_stocks.length - 1];
          owned_stocks.pop();
          break;
        }
      }
    }

    //buy stocks
    //NOTE - add outer loop?
    var new_balance = revenue + balance;
    for(i = 0; i < buy_stocks.length; ++i)
    {
      //use difference to control number to buy
      var desired_num = Math.floor(buy_stocks[i].diff * -100);
      while(desired_num > 0)
      {
        if(buy_stocks[i].price * desired_num <= new_balance)
        {
          buy_stocks[i].num_owned = desired_num;
          owned_stocks.push(buy_stocks[i]);
          new_balance -= buy_stocks[i].price * desired_num;
          break;
        }
        --desired_num;
      }
    }

    //calculate new account value
    var value = 0;
    for(i = 0; i < owned_stocks.length; ++i)
    {
      var id = owned_stocks[i].id;
      var num_owned = owned_stocks[i].num_owned;
      value += data[id][day].Adjusted_close * num_owned;
    }

    //calculate percent gain/loss
    var net_worth = value + new_balance;
    gains_losses.push((1.0 * net_worth) / value_over_time[value_over_time.length - 1] - 1);
    value_over_time.push(net_worth);
        
    //calculate drawdown
    var drawdown = (prev_maxima - net_worth) / prev_maxima;
    if(drawdown < 0)
    {
      prev_maxima = net_worth;
    }
    else if(drawdown > max_drawdown)
    {
      max_drawdown = drawdown;
    }

    //update number of days stocks have been owned
    for(i = 0; i < owned_stocks.length; ++i)
    {
      ++owned_stocks[i].days_owned;
    }

    //update current balance
    balance = new_balance;

    //update stock ownerships
    var owned_array = new Array(num_stocks).fill(0);
    for(i = 0; i < owned_stocks.length; ++i)
    {
      owned_array[owned_stocks[i].id] += owned_stocks[i].num_owned;
    }
    g_OwnedPerDay.push(owned_array);
  }
}