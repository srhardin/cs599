function StartSimulation()
{
    g_StartDate = document.getElementById('startDate').value;
    g_EndDate = document.getElementById('endDate').value;
    
    if(g_Algorithm == "Random")
    {
        run_simulation();
    }
    else if (g_Algorithm == "Alg2")
    {
        
    }
    else if (g_Algorithm == "Alg3")
    {
        
    }
    else if (g_Algorithm == "Alg4")
    {
        
    }
    
    // Update the visuals
    UpdateGraphs();
    SetAllStats();
}

function UpdateGraphs()
{
    avGraph.update(value_over_time);
    lgGraph.update(gains_losses);
}

const avGraph = new AccountValueGraph();
const lgGraph = new GainsLossesHistogram();