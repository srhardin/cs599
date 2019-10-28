/*
    index.js
    Author: Sawyer Goodsel
    Class: cs399f19
    Assignment: Project 5
    Copyright 2019 DigiPen Institute of Technology
*/
let CompanyHash = {};

function combo(thelist)
{
	var idx = thelist.selectedIndex;
	var content = thelist.options[idx].innerHTML;
	console.log(content);
	g_SelectedStock = content;
	UpdateStockGraphs(content);
}


function BuildHash(){
CompanyHash["A"] = 0;
CompanyHash["AAP"] = 0;
CompanyHash["AAPL"] = 0;
CompanyHash["ABBV"] = 0;
CompanyHash["ABC"] = 0;
CompanyHash["ABEV"] = 0;
CompanyHash["ABT"] = 0;
CompanyHash["ACN"] = 0;
CompanyHash["ADBE"] = 0;
CompanyHash["ADI"] = 0;
CompanyHash["ADM"] = 0;
CompanyHash["ADP"] = 0;
CompanyHash["ADSK"] = 0;
CompanyHash["AEE"] = 0;
CompanyHash["AEP"] = 0;
CompanyHash["AFK"] = 0;
CompanyHash["AFL"] = 0;
CompanyHash["AGN"] = 0;
CompanyHash["AIG"] = 0;
CompanyHash["AJG"] = 0;
CompanyHash["AKAM"] = 0;
CompanyHash["ALC"] = 0;
CompanyHash["ALGN"] = 0;
CompanyHash["ALL"] = 0;
CompanyHash["ALXN"] = 0;
CompanyHash["AMAT"] = 0;
CompanyHash["AMCR"] = 0;
CompanyHash["AMD"] = 0;
CompanyHash["AME"] = 0;
CompanyHash["AMGN"] = 0;
CompanyHash["AMH"] = 0;
CompanyHash["AMP"] = 0;
CompanyHash["AMT"] = 0;
CompanyHash["AMTD"] = 0;
CompanyHash["AMZN"] = 0;
CompanyHash["ANET"] = 0;
CompanyHash["ANSS"] = 0;
CompanyHash["ANTM"] = 0;
CompanyHash["AON"] = 0;
CompanyHash["APD"] = 0;
CompanyHash["APH"] = 0;
CompanyHash["APO"] = 0;
CompanyHash["APTV"] = 0;
CompanyHash["ARE"] = 0;
CompanyHash["ARW"] = 0;
CompanyHash["ATO"] = 0;
CompanyHash["ATR"] = 0;
CompanyHash["ATUS"] = 0;
CompanyHash["ATVI"] = 0;
CompanyHash["AVB"] = 0;
CompanyHash["AVGO"] = 0;
CompanyHash["AVY"] = 0;
CompanyHash["AWK"] = 0;
CompanyHash["AXP"] = 0;
CompanyHash["AYI"] = 0;
CompanyHash["AZO"] = 0;
CompanyHash["BA"] = 0;
CompanyHash["BABA"] = 0;
CompanyHash["BAC"] = 0;
CompanyHash["BAH"] = 0;
CompanyHash["BAP"] = 0;
CompanyHash["BAX"] = 0;
CompanyHash["BBT"] = 0;
CompanyHash["BBY"] = 0;
CompanyHash["BDX"] = 0;
CompanyHash["BFAM"] = 0;
CompanyHash["BHGE"] = 0;
CompanyHash["BIDU"] = 0;
CompanyHash["BIIB"] = 0;
CompanyHash["BIL"] = 0;
CompanyHash["BK"] = 0;
CompanyHash["BKNG"] = 0;
CompanyHash["BLK"] = 0;
CompanyHash["BLL"] = 0;
CompanyHash["BMY"] = 0;
CompanyHash["BNDX"] = 0;
CompanyHash["BPL"] = 0;
CompanyHash["BR"] = 0;
CompanyHash["BRK-B"] = 0;
CompanyHash["BRX"] = 0;
CompanyHash["BSX"] = 0;
CompanyHash["BWA"] = 0;
CompanyHash["BX"] = 0;
CompanyHash["BXP"] = 0;
CompanyHash["BZH"] = 0;
CompanyHash["C"] = 0;
CompanyHash["CAG"] = 0;
CompanyHash["CAH"] = 0;
CompanyHash["CAT"] = 0;
CompanyHash["CB"] = 0;
CompanyHash["CBOE"] = 0;
CompanyHash["CBRE"] = 0;
CompanyHash["CBS"] = 0;
CompanyHash["CCI"] = 0;
CompanyHash["CCK"] = 0;
CompanyHash["CCL"] = 0;
CompanyHash["CDNS"] = 0;
CompanyHash["CDW"] = 0;
CompanyHash["CE"] = 0;
CompanyHash["CELG"] = 0;
CompanyHash["CERN"] = 0;
CompanyHash["CFG"] = 0;
CompanyHash["CFR"] = 0;
CompanyHash["CHD"] = 0;
CompanyHash["CHK"] = 0;
CompanyHash["CHRW"] = 0;
CompanyHash["CHTR"] = 0;
CompanyHash["CI"] = 0;
CompanyHash["CINF"] = 0;
CompanyHash["CL"] = 0;
CompanyHash["CLX"] = 0;
CompanyHash["CMA"] = 0;
CompanyHash["CMCSA"] = 0;
CompanyHash["CME"] = 0;
CompanyHash["CMG"] = 0;
CompanyHash["CMI"] = 0;
CompanyHash["CMS"] = 0;
CompanyHash["CNC"] = 0;
CompanyHash["CNI"] = 0;
CompanyHash["CNP"] = 0;
CompanyHash["CNX"] = 0;
CompanyHash["COF"] = 0;
CompanyHash["COO"] = 0;
CompanyHash["COP"] = 0;
CompanyHash["COST"] = 0;
CompanyHash["CP"] = 0;
CompanyHash["CPA"] = 0;
CompanyHash["CPRT"] = 0;
CompanyHash["CRM"] = 0;
CompanyHash["CSCO"] = 0;
CompanyHash["CSGP"] = 0;
CompanyHash["CSL"] = 0;
CompanyHash["CSX"] = 0;
CompanyHash["CTAS"] = 0;
CompanyHash["CTL"] = 0;
CompanyHash["CTLT"] = 0;
CompanyHash["CTSH"] = 0;
CompanyHash["CTVA"] = 0;
CompanyHash["CTXS"] = 0;
CompanyHash["CVS"] = 0;
CompanyHash["CVX"] = 0;
CompanyHash["CXO"] = 0;
CompanyHash["D"] = 0;
CompanyHash["DAL"] = 0;
CompanyHash["DCI"] = 0;
CompanyHash["DD"] = 0;
CompanyHash["DE"] = 0;
CompanyHash["DEI"] = 0;
CompanyHash["DFS"] = 0;
CompanyHash["DG"] = 0;
CompanyHash["DGX"] = 0;
CompanyHash["DHI"] = 0;
CompanyHash["DHR"] = 0;
CompanyHash["DIS"] = 0;
CompanyHash["DISCA"] = 0;
CompanyHash["DISH"] = 0;
CompanyHash["DLR"] = 0;
CompanyHash["DLTR"] = 0;
CompanyHash["DOCU"] = 0;
CompanyHash["DOV"] = 0;
CompanyHash["DOW"] = 0;
CompanyHash["DRE"] = 0;
CompanyHash["DRI"] = 0;
CompanyHash["DSPG"] = 0;
CompanyHash["DTE"] = 0;
CompanyHash["DUK"] = 0;
CompanyHash["DVA"] = 0;
CompanyHash["DXCM"] = 0;
CompanyHash["EA"] = 0;
CompanyHash["EBAY"] = 0;
CompanyHash["ECL"] = 0;
CompanyHash["ED"] = 0;
CompanyHash["EDU"] = 0;
CompanyHash["EFX"] = 0;
CompanyHash["EGP"] = 0;
CompanyHash["EIX"] = 0;
CompanyHash["EL"] = 0;
CompanyHash["ELS"] = 0;
CompanyHash["EMN"] = 0;
CompanyHash["EMR"] = 0;
CompanyHash["ENB"] = 0;
CompanyHash["ENR"] = 0;
CompanyHash["EOG"] = 0;
CompanyHash["EPAM"] = 0;
CompanyHash["EPD"] = 0;
CompanyHash["EQIX"] = 0;
CompanyHash["EQR"] = 0;
CompanyHash["ES"] = 0;
CompanyHash["ESS"] = 0;
CompanyHash["ET"] = 0;
CompanyHash["ETN"] = 0;
CompanyHash["ETR"] = 0;
CompanyHash["EVRG"] = 0;
CompanyHash["EW"] = 0;
CompanyHash["EWBC"] = 0;
CompanyHash["EXAS"] = 0;
CompanyHash["EXC"] = 0;
CompanyHash["EXPD"] = 0;
CompanyHash["EXPE"] = 0;
CompanyHash["EXR"] = 0;
CompanyHash["FANG"] = 0;
CompanyHash["FAST"] = 0;
CompanyHash["FB"] = 0;
CompanyHash["FBHS"] = 0;
CompanyHash["FCX"] = 0;
CompanyHash["FDX"] = 0;
CompanyHash["FE"] = 0;
CompanyHash["FICO"] = 0;
CompanyHash["FIS"] = 0;
CompanyHash["FISV"] = 0;
CompanyHash["FITB"] = 0;
CompanyHash["FLT"] = 0;
CompanyHash["FMX"] = 0;
CompanyHash["FNDA"] = 0;
CompanyHash["FRC"] = 0;
CompanyHash["FRT"] = 0;
CompanyHash["FSLR"] = 0;
CompanyHash["FTV"] = 0;
CompanyHash["GD"] = 0;
CompanyHash["GDDY"] = 0;
CompanyHash["GE"] = 0;
CompanyHash["GILD"] = 0;
CompanyHash["GIS"] = 0;
CompanyHash["GLD"] = 0;
CompanyHash["GLW"] = 0;
CompanyHash["GM"] = 0;
CompanyHash["GNRC"] = 0;
CompanyHash["GOOG"] = 0;
CompanyHash["GOOGL"] = 0;
CompanyHash["GPC"] = 0;
CompanyHash["GPN"] = 0;
CompanyHash["GPS"] = 0;
CompanyHash["GRMN"] = 0;
CompanyHash["GS"] = 0;
CompanyHash["GWW"] = 0;
CompanyHash["HAL"] = 0;
CompanyHash["HAS"] = 0;
CompanyHash["HBAN"] = 0;
CompanyHash["HCA"] = 0;
CompanyHash["HCP"] = 0;
CompanyHash["HD"] = 0;
CompanyHash["HDB"] = 0;
CompanyHash["HES"] = 0;
CompanyHash["HIG"] = 0;
CompanyHash["HLT"] = 0;
CompanyHash["HOLX"] = 0;
CompanyHash["HON"] = 0;
CompanyHash["HPE"] = 0;
CompanyHash["HPQ"] = 0;
CompanyHash["HRL"] = 0;
CompanyHash["HST"] = 0;
CompanyHash["HSY"] = 0;
CompanyHash["HTHT"] = 0;
CompanyHash["HUM"] = 0;
CompanyHash["IAC"] = 0;
CompanyHash["IBM"] = 0;
CompanyHash["ICE"] = 0;
CompanyHash["IDXX"] = 0;
CompanyHash["IEX"] = 0;
CompanyHash["IFF"] = 0;
CompanyHash["ILMN"] = 0;
CompanyHash["INCY"] = 0;
CompanyHash["INFO"] = 0;
CompanyHash["INFY"] = 0;
CompanyHash["INTC"] = 0;
CompanyHash["INTU"] = 0;
CompanyHash["INVH"] = 0;
CompanyHash["IP"] = 0;
CompanyHash["IPHI"] = 0;
CompanyHash["IQV"] = 0;
CompanyHash["IR"] = 0;
CompanyHash["ISRG"] = 0;
CompanyHash["IT"] = 0;
CompanyHash["ITUB"] = 0;
CompanyHash["ITW"] = 0;
CompanyHash["JAZZ"] = 0;
CompanyHash["JBGS"] = 0;
CompanyHash["JD"] = 0;
CompanyHash["JEC"] = 0;
CompanyHash["JKHY"] = 0;
CompanyHash["JNJ"] = 0;
CompanyHash["JOBS"] = 0;
CompanyHash["JPM"] = 0;
CompanyHash["JWN"] = 0;
CompanyHash["K"] = 0;
CompanyHash["KEY"] = 0;
CompanyHash["KEYS"] = 0;
CompanyHash["KHC"] = 0;
CompanyHash["KIM"] = 0;
CompanyHash["KLAC"] = 0;
CompanyHash["KMB"] = 0;
CompanyHash["KMI"] = 0;
CompanyHash["KMX"] = 0;
CompanyHash["KO"] = 0;
CompanyHash["KR"] = 0;
CompanyHash["KSS"] = 0;
CompanyHash["KSU"] = 0;
CompanyHash["L"] = 0;
CompanyHash["LDOS"] = 0;
CompanyHash["LEN"] = 0;
CompanyHash["LH"] = 0;
CompanyHash["LHX"] = 0;
CompanyHash["LII"] = 0;
CompanyHash["LIN"] = 0;
CompanyHash["LLY"] = 0;
CompanyHash["LMT"] = 0;
CompanyHash["LNC"] = 0;
CompanyHash["LNT"] = 0;
CompanyHash["LOGI"] = 0;
CompanyHash["LOW"] = 0;
CompanyHash["LPT"] = 0;
CompanyHash["LRCX"] = 0;
CompanyHash["LSXMA"] = 0;
CompanyHash["LULU"] = 0;
CompanyHash["LUV"] = 0;
CompanyHash["LVS"] = 0;
CompanyHash["LYB"] = 0;
CompanyHash["LYFT"] = 0;
CompanyHash["MA"] = 0;
CompanyHash["MAA"] = 0;
CompanyHash["MAR"] = 0;
CompanyHash["MAS"] = 0;
CompanyHash["MCD"] = 0;
CompanyHash["MCHP"] = 0;
CompanyHash["MCK"] = 0;
CompanyHash["MDLZ"] = 0;
CompanyHash["MDT"] = 0;
CompanyHash["MELI"] = 0;
CompanyHash["MET"] = 0;
CompanyHash["MGM"] = 0;
CompanyHash["MHK"] = 0;
CompanyHash["MIDD"] = 0;
CompanyHash["MKC"] = 0;
CompanyHash["MKTX"] = 0;
CompanyHash["MLM"] = 0;
CompanyHash["MMC"] = 0;
CompanyHash["MMM"] = 0;
CompanyHash["MMP"] = 0;
CompanyHash["MNST"] = 0;
CompanyHash["MO"] = 0;
CompanyHash["MPC"] = 0;
CompanyHash["MPLX"] = 0;
CompanyHash["MRK"] = 0;
CompanyHash["MRVL"] = 0;
CompanyHash["MS"] = 0;
CompanyHash["MSA"] = 0;
CompanyHash["MSCI"] = 0;
CompanyHash["MSFT"] = 0;
CompanyHash["MSI"] = 0;
CompanyHash["MTB"] = 0;
CompanyHash["MTCH"] = 0;
CompanyHash["MTD"] = 0;
CompanyHash["MU"] = 0;
CompanyHash["MXIM"] = 0;
CompanyHash["NBR"] = 0;
CompanyHash["NDAQ"] = 0;
CompanyHash["NEE"] = 0;
CompanyHash["NEM"] = 0;
CompanyHash["NFG"] = 0;
CompanyHash["NFLX"] = 0;
CompanyHash["NI"] = 0;
CompanyHash["NKE"] = 0;
CompanyHash["NNN"] = 0;
CompanyHash["NOC"] = 0;
CompanyHash["NOW"] = 0;
CompanyHash["NSC"] = 0;
CompanyHash["NTAP"] = 0;
CompanyHash["NTRS"] = 0;
CompanyHash["NUE"] = 0;
CompanyHash["NVDA"] = 0;
CompanyHash["NVR"] = 0;
CompanyHash["NWE"] = 0;
CompanyHash["NXST"] = 0;
CompanyHash["NYT"] = 0;
CompanyHash["O"] = 0;
CompanyHash["OAK"] = 0;
CompanyHash["ODFL"] = 0;
CompanyHash["OKE"] = 0;
CompanyHash["OMC"] = 0;
CompanyHash["ORCL"] = 0;
CompanyHash["ORLY"] = 0;
CompanyHash["OUT"] = 0;
CompanyHash["OXY"] = 0;
CompanyHash["PAA"] = 0;
CompanyHash["PAG"] = 0;
CompanyHash["PANW"] = 0;
CompanyHash["PAYX"] = 0;
CompanyHash["PBF"] = 0;
CompanyHash["PCAR"] = 0;
CompanyHash["PCG"] = 0;
CompanyHash["PE"] = 0;
CompanyHash["PEG"] = 0;
CompanyHash["PEP"] = 0;
CompanyHash["PFE"] = 0;
CompanyHash["PFG"] = 0;
CompanyHash["PFGC"] = 0;
CompanyHash["PFPT"] = 0;
CompanyHash["PG"] = 0;
CompanyHash["PGR"] = 0;
CompanyHash["PH"] = 0;
CompanyHash["PKG"] = 0;
CompanyHash["PLD"] = 0;
CompanyHash["PM"] = 0;
CompanyHash["PNC"] = 0;
CompanyHash["PODD"] = 0;
CompanyHash["POOL"] = 0;
CompanyHash["POR"] = 0;
CompanyHash["POST"] = 0;
CompanyHash["PPG"] = 0;
CompanyHash["PPL"] = 0;
CompanyHash["PRU"] = 0;
CompanyHash["PSA"] = 0;
CompanyHash["PSX"] = 0;
CompanyHash["PVH"] = 0;
CompanyHash["PXD"] = 0;
CompanyHash["PYPL"] = 0;
CompanyHash["QCOM"] = 0;
CompanyHash["QQQ"] = 0;
CompanyHash["RBBN"] = 0;
CompanyHash["RCL"] = 0;
CompanyHash["REG"] = 0;
CompanyHash["REGN"] = 0;
CompanyHash["RF"] = 0;
CompanyHash["RIG"] = 0;
CompanyHash["RJF"] = 0;
CompanyHash["RMD"] = 0;
CompanyHash["ROK"] = 0;
CompanyHash["ROP"] = 0;
CompanyHash["ROST"] = 0;
CompanyHash["RPM"] = 0;
CompanyHash["RRC"] = 0;
CompanyHash["RSG"] = 0;
CompanyHash["RTN"] = 0;
CompanyHash["RY"] = 0;
CompanyHash["RYN"] = 0;
CompanyHash["SBAC"] = 0;
CompanyHash["SBUX"] = 0;
CompanyHash["SCHW"] = 0;
CompanyHash["SHLDQ"] = 0;
CompanyHash["SHOP"] = 0;
CompanyHash["SHW"] = 0;
CompanyHash["SIVB"] = 0;
CompanyHash["SJM"] = 0;
CompanyHash["SLB"] = 0;
CompanyHash["SLGN"] = 0;
CompanyHash["SNA"] = 0;
CompanyHash["SNPS"] = 0;
CompanyHash["SO"] = 0;
CompanyHash["SPG"] = 0;
CompanyHash["SPGI"] = 0;
CompanyHash["SPLK"] = 0;
CompanyHash["SPOT"] = 0;
CompanyHash["SPY"] = 0;
CompanyHash["SQ"] = 0;
CompanyHash["SRE"] = 0;
CompanyHash["STI"] = 0;
CompanyHash["STT"] = 0;
CompanyHash["STX"] = 0;
CompanyHash["STZ"] = 0;
CompanyHash["SWK"] = 0;
CompanyHash["SWKS"] = 0;
CompanyHash["SYF"] = 0;
CompanyHash["SYK"] = 0;
CompanyHash["SYMC"] = 0;
CompanyHash["SYY"] = 0;
CompanyHash["T"] = 0;
CompanyHash["TAP"] = 0;
CompanyHash["TD"] = 0;
CompanyHash["TDG"] = 0;
CompanyHash["TDOC"] = 0;
CompanyHash["TEAM"] = 0;
CompanyHash["TEL"] = 0;
CompanyHash["TER"] = 0;
CompanyHash["TFX"] = 0;
CompanyHash["TGT"] = 0;
CompanyHash["TIF"] = 0;
CompanyHash["TJX"] = 0;
CompanyHash["TMO"] = 0;
CompanyHash["TMUS"] = 0;
CompanyHash["TROW"] = 0;
CompanyHash["TRP"] = 0;
CompanyHash["TRV"] = 0;
CompanyHash["TSCO"] = 0;
CompanyHash["TSM"] = 0;
CompanyHash["TSN"] = 0;
CompanyHash["TTC"] = 0;
CompanyHash["TTD"] = 0;
CompanyHash["TTWO"] = 0;
CompanyHash["TWLO"] = 0;
CompanyHash["TWTR"] = 0;
CompanyHash["TXN"] = 0;
CompanyHash["TXT"] = 0;
CompanyHash["UAL"] = 0;
CompanyHash["UDR"] = 0;
CompanyHash["UHS"] = 0;
CompanyHash["ULTA"] = 0;
CompanyHash["UNH"] = 0;
CompanyHash["UNP"] = 0;
CompanyHash["UPS"] = 0;
CompanyHash["USB"] = 0;
CompanyHash["UTX"] = 0;
CompanyHash["V"] = 0;
CompanyHash["VEEV"] = 0;
CompanyHash["VFC"] = 0;
CompanyHash["VICI"] = 0;
CompanyHash["VIG"] = 0;
CompanyHash["VLO"] = 0;
CompanyHash["VMC"] = 0;
CompanyHash["VNO"] = 0;
CompanyHash["VRSK"] = 0;
CompanyHash["VRSN"] = 0;
CompanyHash["VRTX"] = 0;
CompanyHash["VTIP"] = 0;
CompanyHash["VTR"] = 0;
CompanyHash["VZ"] = 0;
CompanyHash["W"] = 0;
CompanyHash["WAB"] = 0;
CompanyHash["WAT"] = 0;
CompanyHash["WBA"] = 0;
CompanyHash["WCG"] = 0;
CompanyHash["WCN"] = 0;
CompanyHash["WDAY"] = 0;
CompanyHash["WDC"] = 0;
CompanyHash["WEC"] = 0;
CompanyHash["WELL"] = 0;
CompanyHash["WEX"] = 0;
CompanyHash["WFT"] = 0;
CompanyHash["WLL"] = 0;
CompanyHash["WLTW"] = 0;
CompanyHash["WM"] = 0;
CompanyHash["WMB"] = 0;
CompanyHash["WMT"] = 0;
CompanyHash["WPC"] = 0;
CompanyHash["WRB"] = 0;
CompanyHash["WRK"] = 0;
CompanyHash["WST"] = 0;
CompanyHash["WY"] = 0;
CompanyHash["XEL"] = 0;
CompanyHash["XLB"] = 0;
CompanyHash["XLNX"] = 0;
CompanyHash["XOM"] = 0;
CompanyHash["XRAY"] = 0;
CompanyHash["XYL"] = 0;
CompanyHash["Y"] = 0;
CompanyHash["YUM"] = 0;
CompanyHash["YUMC"] = 0;
CompanyHash["ZBH"] = 0;
CompanyHash["ZBRA"] = 0;
CompanyHash["ZEN"] = 0;
CompanyHash["ZTS"] = 0;
}
//This returns a promise!

let DateHash = {};
function getData()
{

	BuildHash();
	let CompanyReturn = {};
	let PromiseList = [];
	let CompanyData = new Array();
	let count = 0;
	let AComp = d3.csv("https://raw.githubusercontent.com/srhardin/cs599/master/proj_6/data/" + "A" + ".csv").then(function(data) {
		for(var i = 0; i < data.length; ++i)
		{
			DateHash[data[i]["Date"]] = i;
		}
		//console.log("done");
		//console.log(DateHash);
	});
	
	//This allows us to go through all of the keys in a given object. ezpz
	for (var key in CompanyHash) {
		if (CompanyHash.hasOwnProperty(key)) {
			CompanyHash[key] = count++;
			PromiseList.push(d3.csv("https://raw.githubusercontent.com/srhardin/cs599/master/proj_6/data/" + key + ".csv"));
		}
	}
	//console.log(count);
	//console.log(CompanyHash);
	count = 0;
	//This will resolve the promises in order once they are all finished
	//This means they will be put in alphabetical order, since that is the order we called them in initially
	let completion = new Promise(function (resolve,reject) {
		Promise.all(PromiseList).then(function(data) {
			
			for(var key in DateHash) 
			{
				for(let i = 0; i < data.length; ++i)
				{
					if(data[i].length <= DateHash[key])
					{
						let temp = {};
						temp["Date"] = key;
						temp["Open"] = 0;
						temp["High"] = 0;
						temp["Low"] = 0;
						temp["Close"] = 0;
						temp["Adjusted_close"] = 0;
						temp["Volume"] = 0;
						data[i].push(temp);
					}
					else if(data[i][DateHash[key]].Date != key)
					{
						//console.log(++count);
						let temp = {};
						temp["Date"] = key;
						temp["Open"] = 0;
						temp["High"] = 0;
						temp["Low"] = 0;
						temp["Close"] = 0;
						temp["Adjusted_close"] = 0;
						temp["Volume"] = 0;
						data[i].splice(DateHash[key],0, temp);
						//console.log(temp);
					}
				
				}
			}

			console.log(data);
			CompanyData.push(data);
		}).then(function() {
			CompanyReturn["Hash"] = CompanyHash;
			CompanyReturn["Data"] = CompanyData[0];
			CompanyReturn["Date"] = DateHash;
			resolve(CompanyReturn);
		});
	});
	//return the promise that will resolve into an object that contains the hash and the data
	return completion;
}

function getStockData(stockName, startDate, endDate)
{
	let returnData = [];
	returnData = data[hash[stockName]].slice(DateHash[startDate], DateHash[endDate]);
	let finalData = [];
	for(let i = 0; i < returnData.length; ++i)
	{
		let push = 0;
		push = parseFloat(returnData[i].Close);
		finalData.push(push);
		
	}
	console.log(finalData);
	return finalData;
	
}

function getOwnedData(stockName)
{
	let dataReturn = [];
	for(let i = 0; i < g_OwnedPerDay.length; ++i)
	{
		dataReturn.push(g_OwnedPerDay[i][hash[stockName]]);
	}
	return dataReturn;
}



