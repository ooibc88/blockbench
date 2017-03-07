//
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include "db/db_factory.h"

#include <string>
#include "db/hyperledger_db.h"
#include "db/evm_db.h"

using namespace std;
using ycsbc::DB;
using ycsbc::DBFactory;

DB* DBFactory::CreateDB(utils::Properties& props) {
  if (props["dbname"] == "hyperledger") {
    const string endpoint = props["endpoint"];
    string wl_name = props.GetProperty("workload", "donothing");
    if (wl_name == "smallbank") {
      cout << "Please use the dedicated SmallBank driver for HyperLedger"
           << endl;
      exit(0);
    }
    return new HyperLedgerDB(endpoint, wl_name);
  } else if (props["dbname"] == "ethereum" || props["dbname"] == "parity") {
    const string endpoint = props["endpoint"];
    int deploy_wait = stoi(props.GetProperty("deploy_wait", "20"));
    string wl_name = props.GetProperty("workload", "donothing");
    return new EVMDB(endpoint, props["dbname"], wl_name, deploy_wait);
  } else {
    return NULL;
  }
}
