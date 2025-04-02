import 'package:flutter/material.dart';

import 'devices/main.dart';

class HomePage extends StatelessWidget {
  static const title = "Home";

  const HomePage({super.key});

  @override
  Widget build(BuildContext context) => Padding(
      padding: const EdgeInsets.all(32),
      child: Column(children: <Widget>[Text('foo'), const Devices()]));
}
