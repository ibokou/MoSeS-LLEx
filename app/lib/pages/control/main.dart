import 'package:flutter/material.dart';

class ControlPage extends StatelessWidget {
  static const title = "Control";

  const ControlPage({super.key});

  @override
  Widget build(BuildContext context) => Scaffold(
        body: Center(
            child: Icon(
          Icons.gamepad,
        )),
      );
}
