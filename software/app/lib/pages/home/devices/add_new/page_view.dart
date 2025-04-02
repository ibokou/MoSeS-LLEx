import 'package:app/pages/home/devices/add_new/sub_pages/page_device_type.dart';
import 'package:app/pages/home/devices/add_new/sub_pages/page_nearby_device.dart';
import 'package:app/pages/home/devices/add_new/sub_pages/selection.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class AddNewPageView extends ConsumerWidget {
  const AddNewPageView({super.key});

  static final pageController = PageController();

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    ref.listen<SubPageId>(subPageSelectionProvider,
        (SubPageId? previous, SubPageId next) {
      if (pageController.hasClients) {
        pageController.animateToPage(next.index,
            duration: const Duration(milliseconds: 400),
            curve: Curves.easeInOut);
      }
    });

    return Expanded(
        child: PageView(
            scrollDirection: Axis.horizontal,
            controller: pageController,
            onPageChanged: (int pageIndex) => {
                  ref
                      .read(subPageSelectionProvider.notifier)
                      .set((SubPageId.values[pageIndex])),
                },
            children: [const DeviceTypePage(), const NearbyDevicePage()]));
  }
}
