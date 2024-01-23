import { Midi } from "@tonejs/midi";
import * as Tone from "tone";
import { Frequency, Synth } from "tone";
import { convertMelody, saveFile } from "./arduino";

const [playSymbol, stopSymbol] = [
  `
  <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path d="M8 5v14l11-7z"></path><path d="M0 0h24v24H0z" fill="none"></path></svg>
    `,
  `
  <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path d="M0 0h24v24H0z" fill="none"></path><path d="M6 6h12v12H6z"></path></svg>
  `,
];
const trackSelector = document.getElementById("tracks") as HTMLSelectElement;
const playPauseBtn = document.getElementById(
  "play-toggle"
) as HTMLButtonElement;
const codeTextArea = document.getElementById(
  "arduinoCode"
) as HTMLTextAreaElement;
const downloadBtn = document.getElementById("downloadBtn");
const errorTxt = document.getElementById("error");

const renderError = (msg: string) => {
  errorTxt.innerText = msg;
};
if (!(window.File && window.FileReader && window.FileList && window.Blob)) {
  document.querySelector("#FileDrop #Text").textContent =
    "Reading files not supported by this browser";
} else {
  const fileDrop = document.querySelector("#FileDrop");

  fileDrop.addEventListener("dragover", () => fileDrop.classList.add("Hover"));

  fileDrop.addEventListener("dragleave", () =>
    fileDrop.classList.remove("Hover")
  );

  fileDrop.addEventListener("drop", () => fileDrop.classList.remove("Hover"));

  const fileInput = document.querySelector(
    "#FileDrop input"
  ) as HTMLInputElement;
  fileInput.addEventListener("change", (e) => {
    //get the files
    const target = e.target as HTMLInputElement;
    const files = target.files;
    if (files.length > 0) {
      const file = files[0];
      document.querySelector("#FileDrop #Text").textContent = file.name;
      parseFile(file);
    }
  });
}

let currentMidi: Midi;
let fileName = "";
let code = "";

const renderTracks = () => {
  let options = ``;
  currentMidi.tracks.forEach((track, i) => {
    options += `<option value=${i}> ${i + 1}. ${track.name} </option>`;
  });
  trackSelector.innerHTML = options;
};

const generateCode = () => {
  const selectedTrack = +trackSelector.value;
  const tracks = currentMidi.tracks[selectedTrack].notes;
  code = convertMelody(tracks);
  codeTextArea.value = code;
  downloadBtn.removeAttribute("disabled");
};
downloadBtn.addEventListener("click", () => {
  saveFile(fileName, code);
});
// update code when user changes selection
trackSelector.addEventListener("change", generateCode);

function parseFile(file: File) {
  //read the file
  const reader = new FileReader();
  reader.onload = function (e) {
    //@ts-ignore
    try {
      const midi = new Midi(e.target.result);
      playPauseBtn.removeAttribute("disabled");
      currentMidi = midi;
      renderTracks();
      generateCode();
      fileName = file.name.split(".")[0] + ".ino";
      errorTxt.innerText = "";
    } catch (err) {
      renderError("Error in parsing " + err);
    }
  };
  reader.readAsArrayBuffer(file);
}

const synths: Synth[] = [];

playPauseBtn.addEventListener("click", () => {
  if (currentMidi && !synths.length) {
    playPauseBtn.innerHTML = stopSymbol;
    const now = Tone.now();
    //create a synth for each track
    const track = currentMidi.tracks[+trackSelector.value];
    const synth = new Tone.Synth().toDestination();
    synths.push(synth);

    // synth.onsilence = () => {
    //   playPauseBtn.innerText = playSymbol;
    // };
    track.notes.forEach((note) => {
      try {
        synth.triggerAttackRelease(note.name, note.duration, note.time + now);
      } catch (err) {
        console.error(err);
      }
    });
  } else {
    //dispose the synth and make a new one
    const synth = synths.shift();
    synth.dispose();
    playPauseBtn.innerHTML = playSymbol;
  }
});
