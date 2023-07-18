var express = require('express');
var bodyParser = require('body-parser');
var mysql = require('mysql2');
var path = require('path');
var connection = mysql.createConnection({
                host: '34.172.24.227',
                user: 'root',
                password: 'orange',
                database: 'olympicstokyo',
                multipleStatements: true
});

connection.connect;

var sql_g;
var searchinput_g;


var app = express();

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(express.static(__dirname + '../public'));

app.get('/', function(req, res) {
        res.render('index', { title: 'Olympics Data' });
});

app.post('/adv_query_1', function(req, res) {
    var sql = `SELECT DISTINCT Athletes.Ath_Name, Athletes.Discipline, Athletes.NOC, Coaches.Event, Teams.Name FROM Athletes JOIN Coaches ON Athletes.NOC = Coaches.NOC JOIN Teams ON Teams.NOC = Athletes.NOC AND Teams.Discipline = Athletes.Discipline AND Teams.Event = Coaches.Event WHERE Athletes.Age > 30 AND Athletes.Salary > (SELECT AVG(Avg_Salary) FROM Sport WHERE Discipline = Athletes.Discipline)`;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('adv-q1', {data_: result});
    });
});

app.post('/adv_query_2', function(req, res) {
    var sql = `SELECT Coaches.Coa_Name, Coaches.Discipline, Coaches.Event, SUM(Medals.Num_Gold) AS Total_Gold FROM Coaches JOIN Medals ON Coaches.NOC = Medals.NOC JOIN Teams ON Coaches.NOC = Teams.NOC AND Coaches.Discipline = Teams.Discipline AND Coaches.Event = Teams.Event GROUP BY Coaches.Coa_Name, Coaches.Discipline, Coaches.Event HAVING SUM(Medals.Num_Gold) > 5;`;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('adv-q2', {data_: result});
    });
});

app.post('/update_medals', function(req, res) {
    var sql = `SELECT * FROM Medals WHERE Ath_Name LIKE "%` + searchinput_g + `%" OR NOC LIKE "%` + searchinput_g + `%" ; `;

	sql += `SELECT SUM(Num_Gold) AS Total_Gold FROM Medals WHERE Ath_Name LIKE "%` + searchinput_g + `%" OR NOC LIKE "%` + searchinput_g + `%" ; `;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('update-medal', {data_g: result[1],
					data_med: result[0]});
    });
});

app.post('/update_meds', function(req, res) {
    let medname = req.body.input_ath_name;
    let mednoc = req.body.input_noc;
    let mednumt = req.body.input_num_t;

    var sql1 = `CALL update_medal_count('${medname}', '${mednoc}', '${mednumt}'); `;

    var sql = "CALL update_medal_count(?, ?, ?); ";

    sql += sql_g;

    connection.query(sql, [medname, mednoc, mednumt], function(err, result, fields) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('update-medal', {data_trig: result[0],
					data_med: result[5]});
    });
});

/* for searching */
app.post('/search_bar', function(req, res) {
    let searchinput = req.body.search_input;
    searchinput_g = searchinput;
    /* do something wih the search input */
    sql_g = `SELECT * FROM Athletes WHERE Ath_Name LIKE "%` + searchinput_g + `%" OR NOC LIKE "%` + searchinput_g + `%" OR Discipline LIKE "%` + searchinput_g + `%" ; `;
    
    sql_g += `SELECT * FROM Coaches WHERE Coa_Name LIKE "%` + searchinput_g + `%" OR NOC LIKE "%` + searchinput_g + `%" OR Discipline LIKE "%` + searchinput_g + `%" OR Event LIKE "%` + searchinput_g + `%" ; `;

    sql_g += `SELECT * FROM Sport WHERE Discipline LIKE "%` + searchinput_g + `%" ; `;
    
    sql_g += `SELECT * FROM Teams WHERE Name LIKE "%` + searchinput_g + `%" OR Discipline LIKE "%` + searchinput_g + `%" OR NOC LIKE "%` + searchinput_g + `%" OR Event LIKE "%` + searchinput_g + `%" ; `;

    sql_g += `SELECT * FROM Medals WHERE Ath_Name LIKE "%` + searchinput_g + `%" OR NOC LIKE "%` + searchinput_g + `%" ; `;

    sql_g += `SELECT * FROM Athletes_Coaches WHERE Ath_Name LIKE "%` + searchinput_g + `%" OR Coa_Name LIKE "%` + searchinput_g + `%" ; `;

    sql_g += `SELECT * FROM Team_Coaches WHERE Coa_Name LIKE "%` + searchinput_g + `%" OR  NOC LIKE "%` + searchinput_g + `%" OR Discipline LIKE "%` + searchinput_g + `%" OR Event LIKE "%` + searchinput_g + `%"`;

    connection.query(sql_g, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[0], 
                                         data_coa: result[1],
                                         data_spo: result[2],
                                         data_tea: result[3],
                                         data_med: result[4],
                                         data_ath_coa: result[5],
                                         data_tea_coa: result[6]});
    });
});

app.post('/insert_ath', function(req, res) {
    let athname = req.body.input_ath_name;
    let athnoc = req.body.input_noc;
    let athdis = req.body.input_dis;
    let athage = req.body.input_age;
    let athsal = req.body.input_sal;

    var sql = `INSERT INTO Athletes VALUES ("` + athname + `", "` + athnoc  + `", "` + athdis  + `", ` + athage  + `, ` + athsal  + `) ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[result.length - 7], 
                                         data_coa: result[result.length - 6],
                                         data_spo: result[result.length - 5],
                                         data_tea: result[result.length - 4],
                                         data_med: result[result.length - 3],
                                         data_ath_coa: result[result.length - 2],
                                         data_tea_coa: result[result.length - 1]});
    });
});

app.post('/insert_coa', function(req, res) {
    let coaname = req.body.input_coa_name;
    let coanoc = req.body.input_noc;
    let coadis = req.body.input_dis;
    let coaeve = req.body.input_eve;

    var sql = `INSERT INTO Coaches VALUES ("` + coaname + `", "` + coanoc  + `", "` + coadis  + `", "` + coaeve  + `") ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/insert_spo', function(req, res) {
    let spodis = req.body.input_dis;
    let spoavgsal = req.body.input_avg_sal;
    let spoavgage = req.body.input_avg_age;

    var sql = `INSERT INTO Sport VALUES ("` + spodis + `", ` + spoavgsal  + `, ` + spoavgage + `) ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/insert_tea', function(req, res) {
    let teaname = req.body.input_name;
    let teanoc = req.body.input_noc;
    let teadis = req.body.input_dis;
    let teaeve = req.body.input_eve;

    var sql = `INSERT INTO Teams VALUES ("` + teaname + `", "` + teadis  + `", "` + teanoc  + `", "` + teaeve + `") ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/insert_med', function(req, res) {
    let medname = req.body.input_ath_name;
    let mednoc = req.body.input_noc;
    let mednumt = req.body.input_num_t;
    let mednumg = req.body.input_num_g;
    let mednums = req.body.input_num_s;
    let mednumb = req.body.input_num_b;

    var sql = `INSERT INTO Medals VALUES ("` + medname + `", "` + mednoc  + `", ` + mednumt  + `, ` + mednumg  + `, ` + mednums + `, ` + mednumb + `) ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/insert_ath_coa', function(req, res) {
    let athname = req.body.input_ath_name;
    let coaname = req.body.input_coa_name;

    var sql = `INSERT INTO Athletes_Coaches VALUES ("` + athname + `", "` + coaname + `") ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/insert_tea_coa', function(req, res) {
    let coaname = req.body.input_coa_name;
    let noc = req.body.input_noc;
    let dis = req.body.input_dis;
    let eve = req.body.input_eve;

    var sql = `INSERT INTO Team_Coaches VALUES ("` + coaname + `", "` + noc  + `", "` + dis  + `", "` + eve + `") ; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_ath', function(req, res) {
    let athname = req.body.input_ath_name;
    let athnoc = req.body.input_noc;
    let athdis = req.body.input_dis;
    let athage = req.body.input_age;
    let athsal = req.body.input_sal;
    let name = req.body.edit_name;

    var sql = `UPDATE Athletes SET Ath_Name = "` + athname + `", NOC = "` + athnoc  + `", Discipline = "` + athdis  + `", Age = ` + athage  + `, Salary = ` + athsal  + ` WHERE Ath_Name = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_coa', function(req, res) {
    let coaname = req.body.input_coa_name;
    let coanoc = req.body.input_noc;
    let coadis = req.body.input_dis;
    let coaeve = req.body.input_eve;
    let name = req.body.edit_name;

    var sql = `UPDATE Coaches SET Coa_Name = "` + coaname + `", NOC = "` + coanoc  + `", Discipline = "` + coadis  + `", Event = "` + coaeve  + `" WHERE Coa_Name = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_spo', function(req, res) {
    let spodis = req.body.input_dis;
    let spoavgsal = req.body.input_avg_sal;
    let spoavgage = req.body.input_avg_age;
    let name = req.body.edit_name;

    var sql = `UPDATE Sport SET Discipline = "` + spodis + `", Avg_Salary = ` + spoavgsal  + `, Avg_Age = ` + spoavgage  + ` WHERE Discipline = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_tea', function(req, res) {
    let teaname = req.body.input_name;
    let teanoc = req.body.input_noc;
    let teadis = req.body.input_dis;
    let teaeve = req.body.input_eve;
    let name = req.body.edit_name;

    var sql = `UPDATE Teams SET Name = "` + teaname + `", Discipline = "` + teadis  + `", NOC = "` + teanoc + `", Event = "` + teaeve  + `" WHERE Name = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_med', function(req, res) {
    let medname = req.body.input_ath_name;
    let mednoc = req.body.input_noc;
    let mednumt = req.body.input_num_t;
    let mednumg = req.body.input_num_g;
    let mednums = req.body.input_num_s;
    let mednumb = req.body.input_num_b;
    let name = req.body.edit_name;

    var sql = `UPDATE Medals SET Ath_Name = "` + medname + `", NOC = "` + mednoc  + `", Num_total = ` + mednumt + `, Num_Gold = ` + mednumg + `, Num_Silver = ` + mednums + `, Num_Brozen = ` + mednumb + ` WHERE Ath_Name = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_ath_coa', function(req, res) {
    let athname = req.body.input_ath_name;
    let coaname = req.body.input_coa_name;
    let name = req.body.edit_name;

    var sql = `UPDATE Athletes_Coaches SET Ath_Name = "` + athname + `", Coa_Name = "` + coaname  + `" WHERE Ath_Name = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/edit_tea_coa', function(req, res) {
    let coaname = req.body.input_coa_name;
    let noc = req.body.input_noc;
    let dis = req.body.input_dis;
    let eve = req.body.input_eve;
    let name = req.body.edit_name;

    var sql = `UPDATE Team_Coaches SET Coa_Name = "` + coaname + `", NOC = "` + noc + `", Discipline = "` + dis + `", Event = "` + eve  + `" WHERE Coa_Name = "` + name + `"; `;

    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render('search-result', {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
    });
});

app.post('/delete_ath', function(req, res) {
        var name = req.body.ath_name;

        var sql = `DELETE FROM Athletes WHERE Ath_Name = "` + name + `" ; ` ;
        sql += sql_g;

        connection.query(sql, function(err, result) {
                if (err) {
                    res.send(err)
                    return;
                }
                res.render("search-result", {data_ath: result[1], 
                                         data_coa: result[2],
                                         data_spo: result[3],
                                         data_tea: result[4],
                                         data_med: result[5],
                                         data_ath_coa: result[6],
                                         data_tea_coa: result[7]});
        });

});

app.post('/delete_coa', function(req, res) {
    var name = req.body.coa_name;

    var sql = `DELETE FROM Coaches WHERE Coa_Name = "` + name + `" ; ` ;
    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render("search-result", {data_ath: result[1], 
                                     data_coa: result[2],
                                     data_spo: result[3],
                                     data_tea: result[4],
                                     data_med: result[5],
                                     data_ath_coa: result[6],
                                     data_tea_coa: result[7]});
    });

});

app.post('/delete_spo', function(req, res) {
    var dis = req.body.dis;

    var sql = `DELETE FROM Sport WHERE Discipline = "` + dis + `" ; ` ;
    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render("search-result", {data_ath: result[1], 
                                     data_coa: result[2],
                                     data_spo: result[3],
                                     data_tea: result[4],
                                     data_med: result[5],
                                     data_ath_coa: result[6],
                                     data_tea_coa: result[7]});
    });

});

app.post('/delete_tea', function(req, res) {
    var name = req.body.name_;

    var sql = `DELETE FROM Teams WHERE Name = "` + name + `" ; ` ;
    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render("search-result", {data_ath: result[1], 
                                     data_coa: result[2],
                                     data_spo: result[3],
                                     data_tea: result[4],
                                     data_med: result[5],
                                     data_ath_coa: result[6],
                                     data_tea_coa: result[7]});
    });

});

app.post('/delete_med', function(req, res) {
    var name = req.body.ath_name;

    var sql = `DELETE FROM Medals WHERE Ath_Name = "` + name + `" ; ` ;
    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render("search-result", {data_ath: result[1], 
                                     data_coa: result[2],
                                     data_spo: result[3],
                                     data_tea: result[4],
                                     data_med: result[5],
                                     data_ath_coa: result[6],
                                     data_tea_coa: result[7]});
    });

});

app.post('/delete_ath_coa', function(req, res) {
    var name = req.body.ath_name;

    var sql = `DELETE FROM Athletes_Coaches WHERE Ath_Name = "` + name + `" ; ` ;
    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render("search-result", {data_ath: result[1], 
                                     data_coa: result[2],
                                     data_spo: result[3],
                                     data_tea: result[4],
                                     data_med: result[5],
                                     data_ath_coa: result[6],
                                     data_tea_coa: result[7]});
    });

});

app.post('/delete_tea_coa', function(req, res) {
    var name = req.body.coa_name;

    var sql = `DELETE FROM Team_Coaches WHERE Coa_Name = "` + name + `" ; ` ;
    sql += sql_g;

    connection.query(sql, function(err, result) {
            if (err) {
                res.send(err)
                return;
            }
            res.render("search-result", {data_ath: result[1], 
                                     data_coa: result[2],
                                     data_spo: result[3],
                                     data_tea: result[4],
                                     data_med: result[5],
                                     data_ath_coa: result[6],
                                     data_tea_coa: result[7]});
    });

});

connection.connect(function(err) {
    if (err) throw err;
    console.log("Connected!");
});

app.listen(80, function () {
    console.log('Node app is running on port 80');
});
