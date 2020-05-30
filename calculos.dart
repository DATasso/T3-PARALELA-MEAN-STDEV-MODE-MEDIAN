import 'dart:io';
import 'dart:async';
import 'dart:math';
import 'dart:convert';
import 'package:csv/csv.dart';

main(List arguments) {
  final File file = new File(arguments[0]);
  List listaNem= [];
  List listaRanking = [];
  List listaMatematica = [];
  List listaLenguaje = [];
  List listaCiencia = [];
  List listaHistoria = [];
  num sumaNem = 0;
  num sumaRanking = 0;
  num sumaMatematica = 0;
  num sumaLenguaje = 0;
  num sumaCiencias = 0;
  num sumaHistoria = 0;
  num largoLista = 0;
  Stream<List> inputStream = file.openRead();
  inputStream
      .transform(utf8.decoder)       // Decode bytes to UTF-8.
      .transform(new LineSplitter()) // Convert stream to individual lines.
      .listen((String line) {        // Process results.

       List row = line.split(';');
       anadirSeparados(listaNem, listaRanking, listaMatematica, listaLenguaje, listaCiencia, listaHistoria,row);
       sumaNem += num.parse(row[1]);
       sumaRanking += num.parse(row[2]);
       sumaMatematica += num.parse(row[3]);
       sumaLenguaje += num.parse(row[4]);
       sumaCiencias += num.parse(row[5]);
       sumaHistoria += num.parse(row[6]);
      },
      onDone: () {
        calculos(listaNem, sumaNem,'Nem');
        calculos(listaRanking, sumaRanking,'Ranking');
        calculos(listaMatematica, sumaMatematica,'Matemática');
        calculos(listaLenguaje, sumaLenguaje,'Lenguaje');
        calculos(listaCiencia, sumaCiencias,'Ciencias');
        calculos(listaHistoria, sumaHistoria,'Historia');
      },
      onError: (e) { print(e.toString()); });
  
}

anadirSeparados(List listaNem, List listaRanking, List listaMatematica, List listaLenguaje, List listaCiencia, List listaHistoria, List linea){
  listaNem.add(linea[1]);
  listaRanking.add(linea[2]);
  listaMatematica.add(linea[3]);
  listaLenguaje.add(linea[4]);
  listaCiencia.add(linea[5]);
  listaHistoria.add(linea[6]);
}

calculos(List lista, num suma, String nombre){
  lista.sort();
  var largo = lista.length;
  num media = suma/largo;
  var mediana;
  var aux = largo~/2;
  if (largo.isEven){
    mediana = (lista[aux] + lista[aux-1]);
  }
  else{
    mediana = lista[((largo-1)~/2)];
  }
  var map = Map();
  num sumCuadrados = 0;
  lista.forEach((element){
    num aux = double.parse(element) - media;
    sumCuadrados += aux*aux;
    if(!map.containsKey(element)){
      map[element] = 1;
    }
    else{
      map[element] +=1;
    }
  });
  num stdev = sqrt(sumCuadrados/largo);
  var contadores = map.values.toList();
  var numero = contadores.reduce((curr,next)=> curr > next ? curr : next);
  var moda = map.keys.firstWhere((k) => map[k] == numero, orElse: () => null);
  print('===$nombre=== \nPromedio: $media \nDesviación Estándar: $stdev \nModa: $moda \nMediana:$mediana\n');
}