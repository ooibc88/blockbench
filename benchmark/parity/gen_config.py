import json
import sys
import os

keyfile_folder_path = "./keys"
config_file_path = "./config.toml.template"
genesis_file_path = "./chain_spec.json.template"

num = int(sys.argv[1])
print "Generating config files for %d nodes...\n" % num

key_list = sorted(os.listdir(keyfile_folder_path))
account_list = []
#for key_file in key_list[:num]:
for key_file in key_list:
    decoded = json.loads(open(keyfile_folder_path+"/"+key_file).read())
    account_list.append('0x'+decoded[u'address'])

config_file_template = open(config_file_path).read()
for i in range(0, num):
    config_file_content = config_file_template[:-1] + "\"" + account_list[i] + "\""
    config_file = open("config.toml."+str(i+1), "w")
    config_file.write(config_file_content)
    config_file.close()


print "Generated %d config files, now generating genesis block json file\n" % num
genesis_file_template = open(genesis_file_path).read()
genesis = json.loads(genesis_file_template)
for account in account_list[:num]:
    genesis[u"engine"][u"authorityRound"][u"params"][u"validators"][u"list"].append(account)
    genesis[u"accounts"][account] = {u"balance": u"0x1000000000000000000000000000"}

with open('chain_spec.json', 'w') as f:
    f.write(unicode(json.dumps(genesis)))

print "Done."


