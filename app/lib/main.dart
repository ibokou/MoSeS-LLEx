import 'dart:async';

import 'package:app/pages/control/main.dart';
import 'package:app/pages/home/main.dart';
import 'package:app/pages/record/main.dart';
import 'package:app/pages/topology/main.dart';
import 'package:app/services/bluetooth/ble_service.dart';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';

part 'main.g.dart';

void main() {
  runApp(ProviderScope(child: const App()));
}

class App extends StatelessWidget {
  const App({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'MoSeS-LLEx',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.blueAccent),
        useMaterial3: true,
      ),
      home: const MainMenu(title: 'MoSeS-LLEx'),
    );
  }
}

@riverpod
class FlutterBluePlusState extends _$FlutterBluePlusState {
  @override
  BluetoothAdapterState build() => BluetoothAdapterState.unknown;

  void set(BluetoothAdapterState newAdapterState) => {state = newAdapterState};
}

@riverpod
class BottomNavigationBarIndex extends _$BottomNavigationBarIndex {
  @override
  int build() => 0;

  void set(int newIndex) => {state = newIndex};
}

class MainMenu extends ConsumerWidget {
  const MainMenu({super.key, required this.title});

  final String title;

  static const List<Widget> _screens = <Widget>[
    HomePage(),
    TopologyPage(),
    RecordPage(),
    ControlPage(),
  ];

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    try {
      BLEService.init(ref);
    } catch(ex) {
      return AlertDialog(
          title: const Text('Basic dialog title'),
          content: const Text(
            'Bluetooth is not supported on this device',
          ),
          actions: <Widget>[TextButton(
            style: TextButton.styleFrom(textStyle: Theme.of(context).textTheme.labelLarge),
            child: const Text('Close App'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),],
      );
    }
    
    return Scaffold(
        appBar: AppBar(title: Text(title)),
        body: Center(
          child:
              _screens.elementAt(ref.watch(bottomNavigationBarIndexProvider)),
        ),
        bottomNavigationBar: BottomNavigationBar(
            type: BottomNavigationBarType.fixed,
            onTap: (int index) => {
                  ref.read(bottomNavigationBarIndexProvider.notifier).set(index)
                },
            currentIndex: ref.watch(bottomNavigationBarIndexProvider),
            fixedColor: Colors.blueAccent,
            items: const <BottomNavigationBarItem>[
              BottomNavigationBarItem(
                  label: HomePage.title, icon: Icon(Icons.home)),
              BottomNavigationBarItem(
                  label: TopologyPage.title, icon: Icon(Icons.hive)),
              BottomNavigationBarItem(
                  label: RecordPage.title, icon: Icon(Icons.motion_photos_on)),
              BottomNavigationBarItem(
                  label: ControlPage.title, icon: Icon(Icons.gamepad))
            ]),
      );}
}
