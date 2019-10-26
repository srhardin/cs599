function StartSimulation()
{
    g_StartDate = document.getElementById('startDate').value;
    g_EndDate = document.getElementById('endDate').value;
    
    ClearGlobalData();
    
    run_simulation();
    
    // Update the visuals
    UpdateGraphs();
    SetAllStats();
}

function ClearGlobalData()
{
    value_over_time.length = 0;
    gains_losses.length = 0;
}

function ResetGraphs()
{
    ResetLineGraph();
    ResetHistogram();
}

const avGraph = new AccountValueGraph();