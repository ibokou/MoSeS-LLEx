import 'package:app/pages/home/devices/add_new/button_connect.dart';
import 'package:app/pages/home/devices/add_new/page_view.dart';
import 'package:app/pages/home/devices/add_new/seg_button_sub_page_select.dart';
import 'package:app/pages/home/devices/button_add_new.dart';
import 'package:flutter/material.dart';

class AddNewPage extends StatelessWidget {
  const AddNewPage({super.key});

  @override
  Widget build(BuildContext context) => Scaffold(
      appBar: AppBar(
        title: const Text(AddNewDeviceButton.actionText),
      ),
      body: Padding(
          padding: const EdgeInsets.all(32),
          child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                const SegmentedButtonSubPageSelection(),
                const AddNewPageView(),
                const ConnectButton(),
              ])));
}
