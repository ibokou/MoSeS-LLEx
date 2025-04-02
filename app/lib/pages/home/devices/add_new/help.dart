import 'package:flutter/material.dart';

class Help extends StatelessWidget {
  const Help({super.key});

  static const String _actionText = "Connect";
  static const double _buttonMinSize = 45;

  @override
  Widget build(BuildContext context) => ElevatedButton(
        style: ElevatedButton.styleFrom(
          backgroundColor: Colors.black,
          foregroundColor: Colors.white,
          minimumSize: Size.fromHeight(_buttonMinSize),
        ),
        onPressed: () {
          Navigator.pop(context);
        },
        child: const Text(_actionText),
      );
}
