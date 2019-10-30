function StartSimulation()
{
    g_StartDate = document.getElementById('startDate').value;
    g_EndDate = document.getElementById('endDate').value;
    
    var alg = document.getElementById('algorithm');
    var opt = alg.options[alg.selectedIndex].innerHTML;
    g_OwnedPerDay = [];
    if(opt == "one")
    {
        algorithm1();
    }
    else if (opt == "two")
    {
		algorithm2();
    }
    else if (opt == "three")
    {
        algorithm3();
    }
    else if (opt == "four")
    {
        algorithm4();
    }
    
    // Update the visuals
    UpdateGraphs();
    SetAllStats();
}

function UpdateGraphs()
{
    avGraph.update(value_over_time);
	stkvlGraph.update(getStockData(g_SelectedStock, g_StartDate, g_EndDate));
	stkcntGraph.update(getOwnedData(g_SelectedStock));
    lgGraph.update(gains_losses);

}

function UpdateStockGraphs(selectedStock)
{
	stkvlGraph.update(getStockData(selectedStock, g_StartDate, g_EndDate));
	stkcntGraph.update(getOwnedData(selectedStock));
}

const avGraph = new AccountValueGraph();
const stkvlGraph = new StockValueGraph();
const stkcntGraph = new StockOwnedGraph();
const lgGraph = new GainsLossesHistogram();