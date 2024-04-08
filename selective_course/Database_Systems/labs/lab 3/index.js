const { Client } = require('pg')
const readline = require('readline')
require('dotenv').config();
const client = new Client({
    user: process.env.user,
    host: process.env.host,
    password: process.env.password,
    database: process.env.database,
    port: 5432
})
client.connect();

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

function receive(){
    return new Promise ( (resolve) => {
        rl.question("Type your query below:\n", (string) => {
            if(string.includes('language')){
                string = string.replace("language=", "(language like ");
                string = string.replace("/", " or language like ");
                var stringArray = string.split(/(\s)/);
                var change = 0;
                for(let i = 0;i<stringArray.length;i++){
                    if(stringArray[i]==' ') continue;
                    if(stringArray[i]=='and' && change==1){
                        change=0;
                        stringArray[i-2]+=')'
                    }
                    if(stringArray[i][0]>='A' && stringArray[i][0]<='Z'){
                        change = 1;
                        stringArray[i] = `'%${stringArray[i]}%'`;
                    }
                }
                if(change==1) stringArray.push(')')
                string = stringArray.join('');
            }
            console.log(`select * from "recommendation" where ${string}`);
            client.query(`select * from "recommendation" where ${string}`, (err, res) => {
                if(err) resolve("incorrect query!");
                else{
                    if(res.rowCount==0){
                        resolve("sorry! there's no movie suitable for your preference today :(");
                    }
                    else{
                        const random = res.rows[Math.floor(Math.random()*res.rowCount)];
                        var date = new Date(random.premiere);
                        date = date.getFullYear()+'/'+(date.getMonth()+1)+'/'+date.getDate();
                        resolve(`We recommend you ${random.title} today!
It is a kind of ${random.genre} show which is premiered in ${date}.
It takes you ${random.runningtime} minutes to watch this ${random.language} movie with ${random.rating} rating.
I hope you like today's recommendation and enjoy watching the movie!`);
                    }
                }
            })
        })
    })
}

(async () => {
    console.log("welcome to Daily recommendation on movie!");
    console.log("You can type some constraint below to let us recommend you!(use and to separate your constraint!)")
    console.log("e.g. rating>=5 and language=Spanish/English and runningtime<=80");
    while(true){
        const str = await receive();
        console.log(str);
        if(str!="incorrect query!"){
            break;
        }
    }
})().then(process.exit);
